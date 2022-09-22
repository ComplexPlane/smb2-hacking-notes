#define X86
#define GX_REV 2
#include <dolphin/types.h>
#undef AT_ADDRESS
#define AT_ADDRESS(x)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wkeyword-macro"
#define volatile extern volatile
#pragma clang diagnostic pop
#include <dolphin/gx/GXVert.h>
#undef volatile
#undef AT_ADDRESS
#include <dolphin/os/OSFastCast.h>
#define GEKKO
#include <dolphin.h>
#undef GEKKO
#undef X86

#undef OS_BUS_CLOCK
#undef OS_CORE_CLOCK
#undef OS_TIMER_CLOCK
extern u32 BUS_CLOCK_SPEED;
extern u32 CPU_CLOCK_SPEED;
#define OS_BUS_CLOCK BUS_CLOCK_SPEED
#define OS_CORE_CLOCK CPU_CLOCK_SPEED
#define OS_TIMER_CLOCK (OS_BUS_CLOCK / 4)
