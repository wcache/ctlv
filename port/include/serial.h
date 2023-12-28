#ifndef __TLV_PORT_SERIAL__H__
#define __TLV_PORT_SERIAL__H__

#include "tlv_types.h"

#ifdef __cplusplus
extern "C" {
#endif


#define UART_PORT_LIST(M) \
    M(UART_PORT0) \
    M(UART_PORT1) \
    M(UART_PORT2)

#define UART_PORT_ENUM(e) e,
typedef enum
{
	UART_PORT_LIST(UART_PORT_ENUM)
    UART_PORT_MAX
} uart_port_e;

typedef enum
{
	UART_WORDLENGTH_8,
	UART_WORDLENGTH_9
} uart_wordlength_e;

typedef enum
{
	UART_STOPBITS_0_5,
	UART_STOPBITS_1,
	UART_STOPBITS_1_5,
	UART_STOPBITS_2
} uart_stopbits_e;

typedef enum
{
	UART_PARITY_NO,
	UART_PARITY_EVE,
	UART_PARITY_OD
} uart_parity_e;

typedef enum
{
	UART_MODE_TX = 0x1,
	UART_MODE_RX = 0x2,
	UART_MODE_RX_TX = 0x3
} uart_mode_e;
	
typedef enum
{
	UART_HWFC_None = 0x0,
	UART_HWFC_RTS,
	UART_HWFC_CTS,
	UART_HWFC_RTS_CTS
} uart_hardware_flowcontrol_e;

typedef struct
{
	tlv_uint32_t baud_rate;
	uart_wordlength_e word_length;
	uart_stopbits_e stop_bits;
	uart_parity_e parity;
	uart_mode_e mode;
	uart_hardware_flowcontrol_e hardware_flowcontrol;
    tlv_uint32_t rx_buffer_size;
} uart_cfg_t;

tlv_bool_t serial_init(uart_port_e port, uart_cfg_t *config);
tlv_int32_t serial_read(uart_port_e port, tlv_uint8_t *data, tlv_uint32_t len, tlv_uint32_t timeout);
tlv_uint32_t serial_write(uart_port_e port, tlv_uint8_t *data, tlv_uint32_t len);
void serial_deinit(uart_port_e port);

#ifdef __cplusplus
}
#endif

#endif
