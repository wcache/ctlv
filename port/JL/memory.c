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

#include <stdlib.h>
#include <string.h>
#include "tlv_types.h"

void *platform_memory_alloc(tlv_uint32_t size)
{
    void *ptr = malloc(size);

    if (ptr) 
    {
        memset(ptr, 0, size);
        return ptr;
    } 
    else 
    {
        return NULL;
    }
}

void *platform_memory_calloc(tlv_uint32_t num, tlv_uint32_t size)
{
    void *ptr = malloc(num * size);

    if (ptr) 
    {
        memset(ptr, 0, size);
        return ptr;
    } 
    else 
    {
        return NULL;
    }
}

void platform_memory_free(void *ptr)
{
    free(ptr);
}
