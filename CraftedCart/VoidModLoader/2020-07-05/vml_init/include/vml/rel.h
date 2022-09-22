#ifndef VML_VML_INIT_REL_H
#define VML_VML_INIT_REL_H

#include "mkb2/gamecube.h"
#include "mkb2/rel.h"

#ifdef __cplusplus
extern "C" {
#endif

RelBufferInfo vml_rel_load(char *file_name);
void vml_rel_run_prolog(RelBufferInfo *buffer_info);
void vml_rel_run_epilog(RelBufferInfo *buffer_info);

#ifdef __cplusplus
}
#endif

#endif
