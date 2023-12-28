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
@file      :tlv_iostream.c
@author    :Elian wang (elian.wang@quectel.com)
@brief     :This file shows the tlv usr interface
@version   :0.1
@date      :2022-06-26 14:58:44
@copyright :Copyright (c) 2022
*/

#include <stdio.h>
#include <string.h>
#include "byteorder.h"
#include "tlv_types.h"
#include "tlv.h"
#include "tlv_iostream.h"
#include "serial.h"
#include "thread.h"
#include "logging.h"
#include "memory.h"

#define TLV_PARSE_BUFFER_LEN  4096

typedef struct {
    uart_port_e uart;
    tlv_box_t * read_box;
    tlv_uint32_t tid;
    tlv_uint8_t *tlv_parse_buffer;
    tlv_uint32_t tlv_buffer_write_pos;
} tlv_ios_t;

static void tlv_read_thread(void *pdata);

tlv_uint32_t tlv_init(uart_port_e port, uart_cfg_t *config, tlv_parse_callback parse_cb, tlv_uint16_t * tags_buff, tlv_uint16_t tags_num)
{
    tlv_bool_t serial_is_initialized = TLV_FALSE;

    tlv_ios_t * tlv_ios = (tlv_ios_t *)platform_memory_alloc(sizeof(tlv_ios_t));
    if(!tlv_ios) {
        TLV_LOG("tlv_ios malloc failed");
        goto error;
    }

    tlv_ios->tlv_parse_buffer = (tlv_uint8_t *)platform_memory_alloc(TLV_PARSE_BUFFER_LEN);
    if(!tlv_ios) {
        TLV_LOG("tlv_parse_buffer malloc failed");
        goto error;
    }

    tlv_ios->uart = port;

    tlv_ios->read_box = tlv_box_create();
    if (!tlv_ios->read_box) {
        TLV_LOG("creat tlv recv box failed");
        goto error;
    }

    tlv_ios->read_box->m_parse_cb = parse_cb;

    if (tags_buff != NULL) {
        tlv_set_tags(tlv_ios->read_box, tags_buff, tags_num);
    } else {
        tlv_set_tags(tlv_ios->read_box, emv_tags, sizeof(emv_tags)/sizeof(tlv_uint16_t));
    }

    if((serial_is_initialized = serial_init(port, config)) == TLV_FALSE) {
        TLV_LOG("uart init failed");
        goto error;
    }

    if((tlv_ios->tid = thread_init(tlv_read_thread, (void *)tlv_ios, 10, 1024)) == 0) {
        TLV_LOG("thread init failed");
        goto error;
    }

    return (tlv_uint32_t)tlv_ios;

error:
    if(serial_is_initialized) {
        serial_deinit(port);
    }

    if (tlv_ios->read_box) {
        tlv_box_destroy(tlv_ios->read_box);
    }

    if (tlv_ios->tlv_parse_buffer) {
        platform_memory_free(tlv_ios->tlv_parse_buffer);
    }

    if (tlv_ios) {
        platform_memory_free(tlv_ios);
    }

    return 0;
}

void tlv_deinit(tlv_uint32_t tlv) {
    tlv_ios_t * tlv_ios = (tlv_ios_t*)tlv;

    if(tlv_ios) {
        thread_deinit(tlv_ios->tid);
        serial_deinit(tlv_ios->uart);
        tlv_box_destroy(tlv_ios->read_box);
        platform_memory_free(tlv_ios->tlv_parse_buffer);
        platform_memory_free(tlv_ios);
    }
}

tlv_error_t tlv_send(tlv_uint32_t tlv, send_data_type *send_data_array, tlv_uint16_t array_len)
{
    tlv_ios_t * tlv_ios = (tlv_ios_t*)tlv;
    tlv_uint32_t i = 0;
    tlv_error_t ret;

    if(!tlv_ios) {
        return NOT_INITIALIZED;
    }
    
    tlv_box_t *box = tlv_box_create();
    if(!box) {
        return NOT_INITIALIZED;
    }
    
    for (i=0; i<array_len; i++)
    {
        ret = tlv_set_value(box, send_data_array[i].tage, send_data_array[i].pdata, send_data_array[i].data_len);
        if (ret != NO_ERROR)
        {
            tlv_box_destroy(box);
            return ret;
        }
    }
    
    ret = tlv_box_serialize(box, TAG_LENGTH_ENDIAN);
    if (ret != NO_ERROR) 
    {
        tlv_box_destroy(box);
        return ret;
    }
    
    serial_write(tlv_ios->uart, tlv_box_get_buffer(box), tlv_box_get_size(box));
    
    tlv_box_destroy(box);

    return ret;
}


static void tlv_read_thread(void *pdata) 
{
    tlv_ios_t * tlv_ios = (tlv_ios_t*)pdata;
    tlv_error_t errno = NO_ERROR;
    
    while (1)
    {   
        tlv_uint8_t len = serial_read(tlv_ios->uart, tlv_ios->tlv_parse_buffer + tlv_ios->tlv_buffer_write_pos, TLV_PARSE_BUFFER_LEN-tlv_ios->tlv_buffer_write_pos, -1);
        tlv_ios->tlv_buffer_write_pos += len;
        
        if (len)
        {
            errno = tlv_parse(tlv_ios->read_box, tlv_ios->tlv_parse_buffer, tlv_ios->tlv_buffer_write_pos, TAG_LENGTH_ENDIAN);
            tlv_ios->read_box->m_parse_cb((tlv_uint32_t)tlv_ios, errno, tlv_ios->tlv_parse_buffer, tlv_ios->tlv_buffer_write_pos, tlv_ios->read_box->m_parse_positon);

            if (errno == NO_ERROR)
            {
                tlv_ios->tlv_buffer_write_pos = 0;
            }
            else
            {
                // Update 'tlv_parse_buffer', Copy the remaining data to the 'tlv_parse_buffer' header
                for (tlv_uint32_t i=0; i < tlv_ios->tlv_buffer_write_pos - tlv_ios->read_box->m_parse_positon; i++)
                {
                    tlv_ios->tlv_parse_buffer[i] = tlv_ios->tlv_parse_buffer[i + tlv_ios->read_box->m_parse_positon];
                }
                // Update tlv_ios->tlv_buffer_write_pos
                tlv_ios->tlv_buffer_write_pos = tlv_ios->tlv_buffer_write_pos - tlv_ios->read_box->m_parse_positon;
            }
        }
    }
}

tlv_base_t *tlv_get_value(tlv_uint32_t tlv)
{
    tlv_ios_t * tlv_ios = (tlv_ios_t*)tlv;
    if(!tlv_ios) {
        return NULL;
    }

    return tlv_box_get_value(tlv_ios->read_box);
}

void tlv_dest_value(tlv_uint32_t tlv) {
    tlv_ios_t * tlv_ios = (tlv_ios_t*)tlv;
    if(!tlv_ios) {
        return;
    }

    tlv_box_dest_value(tlv_ios->read_box);
}
