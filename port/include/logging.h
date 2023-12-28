#ifndef __TLV_PORT_LOGGING__H__
#define __TLV_PORT_LOGGING__H__

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
	
#define TLV_LOG(fmt, ...)   printf("[TLV][%s][%d]"fmt"\r\n", __func__, __LINE__, ##__VA_ARGS__);

#ifdef __cplusplus
}
#endif

#endif
