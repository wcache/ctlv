#ifndef __TLV_PORT_THREAD__H__
#define __TLV_PORT_THREAD__H__

#include "tlv_types.h"

#ifdef __cplusplus
extern "C" {
#endif
	
tlv_uint32_t thread_init(void (*task)(void *p_arg),
                   void *p_arg,
                   tlv_uint8_t prio,
                   tlv_uint32_t stksize);

void thread_deinit(tlv_uint32_t tid);

#ifdef __cplusplus
}
#endif

#endif
