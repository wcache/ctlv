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
#include <stdlib.h>
#include "tlv_types.h"
#include "key_list.h"
#include "memory.h"

key_list_t *key_list_create(value_releaser releaser) 
{
    key_list_t * list = (key_list_t * )platform_memory_alloc(sizeof(key_list_t));
	if (list == NULL)
	{
		return NULL;
	}
    list->count = 0;
    list->header = NULL;
    list->releaser = releaser;
    return list;
}

tlv_bool_t key_list_destroy(key_list_t *list) 
{
    key_list_node_t *current = list->header;    
    while (current != NULL) 
	{
        key_list_node_t *next = current->next;
        list->releaser(current->value);  
        platform_memory_free(current);        
        current = next;
    }
    platform_memory_free(list);
    return TLV_TRUE;
}

tlv_uint32_t key_list_count(key_list_t *list) 
{
    return list->count;    
}

tlv_int32_t key_list_keyset(key_list_t *list, tlv_int32_t* array, tlv_uint32_t array_size)
{    
	tlv_uint32_t i = 0;
    key_list_node_t *current = list->header;
	
    if (array_size < list->count) 
	{
        return -1;
    }
    
    while (current != NULL) 
	{
        array[i] = current->key; 
        current = current->next;
        i++;
    }

    return i; 
}

static key_list_node_t* key_list_get_node(key_list_t *list, tlv_int32_t key) 
{
    key_list_node_t *current = list->header;    

    while (current != NULL) 
	{
        if (key_compare(key, current->key)) 
		{
            return current;
        }       
        current = current->next;
    }
    return NULL;
}

static tlv_bool_t key_list_remove_node(key_list_t *list, key_list_node_t *node) 
{
    if (node == list->header) 
	{
        list->header = node->next;
    }
    else 
	{
        node->prev->next = node->next;
    }

    if (node->next != NULL) 
	{
        node->next->prev = node->prev;                        
    }         

    list->releaser(node->value); 

    platform_memory_free(node);
    list->count--;

    return TLV_TRUE;
}

tlv_bool_t key_list_find_key(key_list_t *list, tlv_int32_t key) 
{
    return key_list_get_node(list, key) != NULL;
}

tlv_bool_t key_list_add(key_list_t *list, tlv_int32_t key, value_t value)
{
    key_list_node_t* new_node = NULL;
    key_list_node_t* p = NULL;

#if 0
    if (key_list_find_key(list, key)) 
	{
        return TLV_FALSE;
    }
#endif

    new_node = platform_memory_calloc(1, sizeof(key_list_node_t));
    if (new_node == NULL) 
	{
        return TLV_FALSE;
    }

    new_node->key = key;
    new_node->value = value;
    new_node->prev = NULL;
    new_node->next = NULL;

    if (list->header == NULL) 
	{
        list->header = new_node;
    }
    else 
	{
        // Find the last node
        p = list->header;
        while(p->next != NULL) 
		{
            p = p->next;
        }
        p->next = new_node;
        new_node->prev = p;
    }
     
    list->count++;

    return TLV_TRUE;   
}

tlv_bool_t key_list_get(key_list_t *list, tlv_int32_t key, value_t *value) 
{
    key_list_node_t* node = key_list_get_node(list, key);
	
    if (node == NULL) 
	{
        return TLV_FALSE;
    }
    *value = node->value;
    return TLV_TRUE;
}

tlv_bool_t key_list_edit(key_list_t *list, tlv_int32_t key, value_t value)
{
    key_list_node_t* node = key_list_get_node(list, key);
    if (node == NULL) 
	{
        return TLV_FALSE;
    }
    node->value = value;
    return TLV_TRUE;
}

tlv_bool_t key_list_delete(key_list_t *list, tlv_int32_t key) 
{
    key_list_node_t* node = key_list_get_node(list, key);
    if (node == NULL) 
	{
        return TLV_FALSE;
    }

    return key_list_remove_node(list, node);
}
