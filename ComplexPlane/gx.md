# Vertex attr formats

It appears that three predefined vertex attributes are defined in three slots.

These are set in function at 0x80144133 (ghidra+ram) using `GXSetVtxAttrFmtv`

## `GX_VTXFMT0`

## `GX_VTXFMT1`

## `GX_VTXFMT7`

Vertices appear to have 3 position floats and 2 texture coord floats. This kind of makes sense for 2D UI elements, and it's used in `draw_debugtext()`. Maybe it's also used in `draw_ui()` in general (which is `draw_debugtext()`'s parent).

# TEV

```c
GX_LoadTexObj(node->texture, GX_TEXMAP0);
GX_SetTevOp(GX_TEVSTAGE0, GX_MODULATE);
GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
```

CraftedCart:

> I guess that's how you'd configure the tev
> Sets the first stage of the TEV to take inputs for a tex coordinates and a texture map, as well as color0+alpha0, and modulate them before sending it to the next tev stage
> 
> Assuming the tev has been configured to only have one stage, that would send it to the render output
I think

# FIFOs

Instead of the CPU writing GX commands into a FIFO and the GP reading the commands from the same FIFO, one FIFO is attached only to the CPU, and one is only attached to the GP. This is called "multi-buffered mode" as opposed to "immediate mode" where there's just one shared, blocking FIFO connected to both the CPU and GP.

The CPU writes commands during frame `n`, while the GP renders the commands in its FIFO from frame `n - 1`. The FIFO attachments alternate every frame.

My guess is this improves throughput because the GP will not ever need to wait for the CPU to write GX commands to the FIFO; the CPU can write GX commands near the end of its frame without worrying about whether the GP will have stuff to process, while the GP can process an entire FIFO full of GX commands from the previous frame unblocked. However, this technically introduces an additional frame of latency over rendering with a single FIFO per frame.

# FIFO size/addresses

Default FIFOs appear to be 1 MiB each (0x100000)! What happens if we make these smaller?

* Main game seems to work fine with half that size, but some party games crash (one bottleneck I found was monkey bowling with Gongon will crash with that size)
* Also the titlescreen cutscene... rip

The two GXFifoObj's used by the game are stored at 0x80147498 (the default one returned by GXInit) and 0x8018fe80. They have size 128.

The FIFO buffers themselves are allocated at 0x815d5b80 (GC) and 0x814d5b80 (GC) (they have size 0x100000).

# Draw waiting and callbacks

Been trying to work out precisely what's going on with the `gp_wait` function...

It appears to poll until the GP is done by checking whether a flag is set by function 80064c5c,
which I bet is a callback function for a GP interrupt.

A pointer to function 80064c5c is passed to function 8002c944, which previously has an incorrect label which I've deleted. I'm pretty dang sure this function is `GXSetDrawDoneCallback`, but would have to verify.

Okay yeah, I'm very sure it's `GXSetDrawDoneCallback`, labelled

# gx_start_new_frame and GXCopyDisp

I'm kind of confused as to why `GXCopyDisp` is called _after_ swapping the GP and CPU FIFOs in `gx_start_new_frame`. If I understand correctly, this should make `GXCopyDisp` one of the first GP commands for the current frame, which should copy the _previous_ frame out of the EFB once the FIFOs are swapped again and the GP is running this FIFO. I'd think this would effectively add another frame of latency, but I don't see how this would help performance at all; each frame, you're still waiting for a GXCopyDisp plus the new GX commands in `gp_wait`, why would this increase throughput?

I'd like to try moving `GXCopyDisp` to the top of `gx_start_new_frame` to see if it reduces latency by a frame without breaking anything or slowing anything down...

Side note: it seems like the game is also buffering inputs for a frame as well or something, because when I modify the ball's y value in memory it takes 3 frames to draw the result, but it seems to take 4 frames to draw the result of a controller input...

So I tried moving GXCopyDisp to the start of `gx_start_new_frame` but it didn't seem to make a difference. I don't really understand why. Maybe an investigation for another day.

# Overall material stuff (Jasper)

Right, so the GameCube / Wii were built on a semi-programmable GPU. Like many things, narrowing down what is and isn't a shader is more a nebulous definition and argument than you might think, and one I sort of don't want to get into, which is why I said "the definition changes depending on what you're asking"

But each model defines a number of shader-like TEV stages, which are expressions of the form "O = lerp(A, B, C) + D". where A, B, C, D are picked from some constant collection of inputs
Indirect texturing / displacement mapping is a special unit in the hardware.

Sure. The line between programmable and fixed is blurry
One of the inputs can be the "texture sample", which is normally of the form "S = texture(textureIdForTheTevStage, texCoordForTheTevStage);"
Indirect texturing is a special mode you can enable which allows it to look like:
S1 = texture(indirectTextureIdForTheTevStage, indirectTexCoordForTheTevStage).abg;
S = texture(textureIdForTheTevStage, texCoordForTheTevStage + (S1 * someNonsenseIWontGetIntoHere));
in my video where I talk about "dialing in the displacement map", that's the someNonsense.

But this is a special hardware block, this was the most they had
And you absolutely couldn't run arbitrary math or trig on the GPU. Linear expressions were the most you had
noclip takes this definition (I call it a "GXMaterial", which is the set of all settings that affect my shader generation), and generates a GLSL shader from it