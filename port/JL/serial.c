/*
 * Copyright (c) Quectel Wireless Solution, Co., Ltd.All Rights Reserved.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * 
 */

/*
@file      :memory.c
@author    :Chavis Chen (chavis.chen@quectel.com)
@brief     :This file shows the platform memory interface
@version   :0.1
@date      :2023-12-14 13:14:44
@copyright :Copyright (c) 2022
*/

#include "tlv_types.h"
#include "serial.h"
#include "asm/uart_dev.h"
#include "asm/gpio.h"
#include "os/os_api.h"
#include "logging.h"

typedef struct {
    uart_bus_t *uart_bus;
    struct uart_platform_data_t u_arg;
    OS_SEM rx_sem;
    tlv_uint8_t rx_sem_created;
} uart_t;

#if defined CONFIG_CPU_BR28 // AC701N
#define UART0_PIN_DEF IO_PORTA_05, IO_PORTA_06
#define UART1_PIN_DEF IO_PORTB_05, IO_PORTB_06
#define UART2_PIN_DEF IO_PORTA_03, IO_PORTA_04
#elif defined CONFIG_CPU_BD19 // AC632N
#define UART0_PIN_DEF IO_PORTA_05, IO_PORTA_06
#define UART1_PIN_DEF IO_PORTB_05, IO_PORTB_06
#define UART2_PIN_DEF IO_PORTA_03, IO_PORTA_04
#else
#error Unsupported platform by tlv.
#endif

static uart_t uart[UART_PORT_MAX] = {
    [UART_PORT0] = {NULL, {UART0_PIN_DEF}, {0}, 0},
    [UART_PORT1] = {NULL, {UART1_PIN_DEF}, {0}, 0},
    [UART_PORT2] = {NULL, {UART2_PIN_DEF}, {0}, 0}
};

static uart_t *uart_find_by_port(uart_port_e port) {
    if(port >= UART_PORT_MAX) {
        return NULL;
    }

    return &uart[port];
}

static uart_t *uart_find_by_bus(uart_bus_t *uart_bus) {
    for(int port = 0; port < UART_PORT_MAX; port++) {
        if(uart[port].uart_bus == uart_bus)
            return &uart[port];
    }

    return NULL;
}

static void uart_isr(void *ut_bus, u32 status) {
    if (status != UT_TX) {
        uart_t * _uart = uart_find_by_bus((uart_bus_t *)ut_bus);
        if(_uart) {
            os_sem_post(&_uart->rx_sem);
        }
    }
}

tlv_bool_t serial_init(uart_port_e port, uart_cfg_t *config) {
    uart_t * _uart = NULL;

    if(port >= UART_PORT_MAX || !config) {
        goto error;
    }

    _uart = uart_find_by_port(port);
    if(!_uart) {
        goto error;
    }

    if(os_sem_create(&_uart->rx_sem, 0)) {
        goto error;
    }

    _uart->rx_sem_created = 1;

    void * rx_cbuf = platform_memory_calloc(1, config->rx_buffer_size);
    if(!rx_cbuf) {
        goto error;
    }

    _uart->u_arg.rx_cbuf = rx_cbuf;
    _uart->u_arg.rx_cbuf_size = config->rx_buffer_size;
    _uart->u_arg.frame_length = config->rx_buffer_size;
    _uart->u_arg.rx_timeout = 1;
    _uart->u_arg.isr_cbfun = uart_isr;
    _uart->u_arg.baud = config->baud_rate;
    _uart->u_arg.is_9bit = config->word_length;

    _uart->uart_bus = uart_dev_open(&_uart->u_arg);
    if (_uart->uart_bus != NULL) {
        TLV_LOG("uart_dev_open() success: %08X", _uart->uart_bus);
    } else {
        goto error;
    }

    return TLV_TRUE;

error:
    TLV_LOG("uart_dev_open() failed");
    if(_uart) {
        if(_uart->rx_sem_created) {
            os_sem_del(&_uart->rx_sem, OS_DEL_ALWAYS);
            _uart->rx_sem_created = 0;
        }

        if(_uart->u_arg.rx_cbuf) {
            platform_memory_free(_uart->u_arg.rx_cbuf);
            _uart->u_arg.rx_cbuf = NULL;
        }

        _uart->uart_bus = NULL;
    }
    return TLV_FALSE;
}

tlv_int32_t serial_read(uart_port_e port, tlv_uint8_t *data, tlv_uint32_t len, tlv_uint32_t timeout) {
    uart_t * _uart = NULL;

    _uart = uart_find_by_port(port);
    if(!_uart) {
        return 0;
    }

    if(_uart->uart_bus) {
        int JL_timeout;
        tlv_uint32_t nread = 0;

        if(timeout == -1) {
            JL_timeout = 0;
        } else if(timeout == 0) {
            JL_timeout = 1;
        } else {
            JL_timeout = timeout;
        }

        nread = _uart->uart_bus->read(data, len, 1);

        if(!nread) {
            if(os_sem_pend(&_uart->rx_sem, JL_timeout)) {
                return 0;
            }

            nread = _uart->uart_bus->read(data, len, 1);
        } else {
            os_sem_set(&_uart->rx_sem, 0);
        }

        return nread;
    }

    return 0;
}

tlv_uint32_t serial_write(uart_port_e port, tlv_uint8_t *data, tlv_uint32_t len) {
    uart_t * _uart = NULL;

    _uart = uart_find_by_port(port);
    if(!_uart) {
        return 0;
    }

    if(_uart->uart_bus) {
        _uart->uart_bus->write(data, len);
        return len;
    }

    return 0;
}

void serial_deinit(uart_port_e port) {
    uart_t * _uart = NULL;

    _uart = uart_find_by_port(port);
    if(!_uart) {
        return;
    }

    if(_uart->uart_bus) {
    #if defined CONFIG_CPU_BR28
        uart_dev_close(&_uart->u_arg, _uart->uart_bus);
    #elif defined CONFIG_CPU_BD19
        uart_dev_close(_uart->uart_bus);
    #else
    #error Unsupported platform by tlv.
    #endif
        _uart->uart_bus = NULL;
    }

    if(_uart->rx_sem_created) {
        os_sem_del(&_uart->rx_sem, OS_DEL_ALWAYS);
        _uart->rx_sem_created = 0;
    }

    if(_uart->u_arg.rx_cbuf) {
        platform_memory_free(_uart->u_arg.rx_cbuf);
        _uart->u_arg.rx_cbuf = NULL;
    }
}