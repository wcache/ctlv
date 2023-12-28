#ifndef __TLV_IOSTREAM_H__
#define __TLV_IOSTREAM_H__

#include "tlv_types.h"
#include "tlv.h"
#include "serial.h"

#ifdef __cplusplus
extern "C" {
#endif
	
typedef struct _send_data 
{
	tlv_uint8_t *pdata;
	tlv_uint32_t data_len;
	tlv_uint16_t tage;
}send_data_type;

tlv_uint32_t tlv_init(uart_port_e port, uart_cfg_t *config, tlv_parse_callback parse_cb, tlv_uint16_t * tags_buff, tlv_uint16_t tags_num);
tlv_base_t *tlv_get_value(tlv_uint32_t tlv);
void tlv_dest_value(tlv_uint32_t tlv);
tlv_error_t tlv_send(tlv_uint32_t tlv, send_data_type *send_data_array, tlv_uint16_t array_len);

#ifdef __cplusplus
}
#endif

#endif
