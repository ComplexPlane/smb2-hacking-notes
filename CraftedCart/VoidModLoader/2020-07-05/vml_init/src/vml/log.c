#include "vml/log.h"
#include "mkb2/gamecube.h"
#include <stdarg.h>
#include <stdio.h>

_Noreturn void vml_panic(char *file, u32 line, char *fmt, ...) {
    OSReport("[VML/FATAL] Panicked at %s:%d", file, line);

    va_list args;
    va_start(args, fmt);

    char msg[2048];

    vsprintf(msg, fmt, args);

    OSReport("[VML/FATAL] %s", msg);
    OSReport("[VML/FATAL] We are hanging");

    va_end(args);

    while (TRUE) {}
}
