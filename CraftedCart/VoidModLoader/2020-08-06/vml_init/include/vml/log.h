#ifndef VML_VML_INIT_LOG_H
#define VML_VML_INIT_LOG_H

#include "mkb2/gamecube.h"

#ifdef __cplusplus
extern "C" {
#endif

_Noreturn void vml_panic(char *file, u32 line, char *fmt, ...);

#define vml_info(...) OSReport("[VML/INFO] " __VA_ARGS__)
#define vml_warn(...) OSReport("[VML/WARN] " __VA_ARGS__)
#define vml_error(...) OSReport("[VML/ERROR] " __VA_ARGS__)
#define vml_fatal(...) vml_panic(__FILE__, __LINE__, __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif
