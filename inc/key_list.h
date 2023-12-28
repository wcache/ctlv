/*
 *  COPYRIGHT NOTICE  
 *  Copyright (C) 2015, Jhuster, All Rights Reserved
 *  Author: Jhuster(lujun.hust@gmail.com)
 *  
 *  https://github.com/Jhuster/TLV
 *   
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published
 *  by the Free Software Foundation; either version 2.1 of the License, 
 *  or (at your option) any later version.
 */
#ifndef __KEY_LIST_H__
#define __KEY_LIST_H__


#include "tlv_types.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct _value {
    void *value;
} value_t;

typedef void (*value_releaser)(value_t value);

#define key_compare(a, b) ((a==b)?1:0)

typedef struct key_list_node { 
    tlv_int32_t key;
    value_t value;
    struct key_list_node *prev;
    struct key_list_node *next;
} key_list_node_t;

typedef struct key_list {
    tlv_uint32_t count;
    key_list_node_t *header;      
    value_releaser releaser;
} key_list_t;

key_list_t *key_list_create(value_releaser releaser);
tlv_bool_t key_list_destroy(key_list_t *list);

tlv_uint32_t key_list_count(key_list_t *list);
tlv_int32_t key_list_keyset(key_list_t *list, tlv_int32_t* array, tlv_uint32_t array_size);
tlv_bool_t key_list_find_key(key_list_t *list, tlv_int32_t key);

tlv_bool_t key_list_add(key_list_t *list, tlv_int32_t key, value_t value);
tlv_bool_t key_list_get(key_list_t *list, tlv_int32_t key, value_t *value);
tlv_bool_t key_list_edit(key_list_t *list, tlv_int32_t key, value_t value);
tlv_bool_t key_list_delete(key_list_t *list, tlv_int32_t key);


#ifdef __cplusplus
}
#endif

#endif
