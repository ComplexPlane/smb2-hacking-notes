There appears to be some sort of separate math library embedded in SMB1+SMB2. A good portion of it was likely written in assembly by hand as it makes extensive use of specialized Gekko floating point instructions.

Currently the functions are labelled prefixed with `math_`

# Matrices

The library operates on temporary 3x4 matrices (`Mtx`, aka `float[3][4]`) that are stored in the locked cache. "Matrix A" and "Matrix B" are stored at the beginning of the locked cache, i.e. at 0xE0000000 and 0xE0000030. Most matrix operations mutate and/or utilize Matrix A, but some functions use Matrix B as well.

Matrix A and Matrix B are referred to as `a_mtx` and `b_mtx` in the function names. For example, `math_invert_a_mtx` will invert Matrix A, and `math_set_b_mtx_a_mtx` will copy Matrix A into Matrix B. To copy between a `Mtx` in real memory and Matrix A, there's `math_get_a_mtx` and `math_set_a_mtx`.

There is also a "matrix stack" for storing intermediate results. The current matrix stack pointer is located at 0xE0000094 (I don't know whether or not the matrix stack is also in the locked cache or not). `math_pop_a_mtx` pops the latest matrix from the matrix stack into Matrix A, for example. I don't think any mathutil functions directly access values on the stack besides pushing and popping into Matrix A and Matrix B.

# Other naming conventions

MANY OF THESE ARE OBSOLETE NOW

* `_v` -> Functions which take a pointer to a Vec3f, as opposed to taking three separate x, y, z arguments. For any given `_v` function, there is often an otherwise identical variant that takes x, y, z arguments and doesn't end with `_v`
* `mtx` -> 3x4 matrix (aka `Mtx` aka `float[3][4])
* `tf` -> transform
* `neg_tf` -> Inverse transform
* `ray` -> Directional line given by a start point and an end point (Vec3f's)
* `sq` -> Refers to the square 3x3 (non-translational) portion of a 3x4 matrix
* `trans` -> Usually refers to setting the 3x1 translational part of Matrix A
* `translate` -> Usually refers to setting a matrix to the identity matrix but with the translational part to the specified vector
* `c` -> Constant, for example `math_scale_a_mtx_sq_c` scales the square portion of Matrix A by a single scalar value

# TODO

* Relabel the `trans` functions
* Run an auto-analysis after labelling function signatures in SMB2 (just to be safe)
* Properly label types for 16-bit rotation functions
<!-- * Import yoshimaster's new symbol map with "mathutil" -> "mathutil" rename -->
<!-- * Rename SMB2 mathutil functions to "mathutil" to match SMB1's new symbols -->
* Some SMB2 mathutil functions may have been ported wrong... should be on the lookout
* Clarify some functions where the argument is meant to be implied in the name like `math_set_a_mtx_mult_by_a_mtx` vs `math_set_a_mtx_multi_a_mtx_by`
* Port function signatures over to SMB1 (but then again there's a lot of stuff that needs to be ported to SMB1 in general...)
* Figure out what the `xz` functions are doing
* neg -> inv / inverse