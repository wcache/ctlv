#ifndef __TLV_PORT_MEMORY__H__
#define __TLV_PORT_MEMORY__H__

#include "tlv_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void *platform_memory_alloc(tlv_uint32_t size);
void *platform_memory_calloc(tlv_uint32_t num, tlv_uint32_t size);
void platform_memory_free(void *ptr);

#ifdef __cplusplus
}
#endif

#endif
