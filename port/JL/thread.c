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
@file      :thread.c
@author    :Elian wang (elian.wang@quectel.com)
@brief     :This file shows the platform thread interface
@version   :0.1
@date      :2022-06-26 13:14:44
@copyright :Copyright (c) 2022
*/
#include "tlv_types.h"
#include "memory.h"
#include "os/os_api.h"
#include "logging.h"
#include <stdio.h>

tlv_uint32_t thread_init(void (*task)(void *p_arg),
                   void *p_arg,
                   tlv_uint8_t prio,
                   tlv_uint32_t stksize)
{
    tlv_str_t thread_name = platform_memory_alloc(16);
    if(!thread_name) {
        goto error;
    }

    sprintf(thread_name, "tlv_%08x", thread_name);

    if(os_task_create(task, p_arg, prio, stksize, 0, thread_name)) {
        goto error;
    }

    return (tlv_uint32_t)thread_name;

error:
    if(thread_name) {
        platform_memory_free(thread_name);
    }

    return 0;
}

void thread_deinit(tlv_uint32_t tid) {
    tlv_str_t thread_name = (tlv_str_t)tid;

    if(thread_name) {
        os_task_del(thread_name);
        platform_memory_free(thread_name);
    }
}
