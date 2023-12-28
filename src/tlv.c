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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tlv_types.h"
#include "tlv.h"
#include "key_list.h"
#include "byteorder.h"
#include "memory.h"


tlv_uint16_t emv_tags[EMV_TAGS_LENTH] = {
    0x9F01, // Acquirer Identifier  
    0x9F02, // Amount, Authorised (Numeric)
    0x9F03, // Amount, Other (Numeric)
    0x9F04, // Amount, Other (Binary)
    0x9F05, // Application Discretionary Data
    0x9F06, // Application Identifier (AID) - terminal
    0x9F07, // Application Usage Control
    0x9F08, // Application Version Number
    0x9F09, // Application Version Number
    0x9F0B, // Cardholder Name Extended
    0xBF0C, // FCI Issuer Discretionary Data
    0x9F0D, // Issuer Action Code - Default
    0x9F0E, // Issuer Action Code - Denial
    0x9F0F, // Issuer Action Code - Online
    0x9F10, // Issuer Application Data
    0x9F11, // Issuer Code Table Index
    0x9F12, // Application Preferred Name
    0x9F13, // Last Online Application Transaction Counter (ATC) Register
    0x9F14, // Lower Consecutive Offline Limit
    0x9F15, // Merchant Category Code
    0x9F16, // Merchant Identifier,
    0x9F17, // Personal Identification Number (PIN) Try Counter
    0x9F18, // Issuer Script Identifier
    0x9F1A, // Terminal Country Code
    0x9F1B, // Terminal Floor Limit
    0x9F1C, // Terminal Identification
    0x9F1D, // Terminal Risk Management Data
    0x9F1E, // Interface Device (IFD) Serial Number
    0x9F1F, // Track 1 Discretionary Data
    0x5F20, // Cardholder Name
    0x9F21, // Transaction Time
    0x9F22, // Certification Authority Public Key Index
    0x9F23, // Upper Consecutive Offline Limit
    0x5F24, // Application Expiration Date
    0x5F25, // Application Effective Date
    0x9F26, // Application Cryptogram
    0x9F27, // Cryptogram Information Data
    0x5F28, // Issuer Country Code
    0x5F2A, // Transaction Currency Code
    0x5F2D, // Language Preference
    0x9F2E, // Integrated Circuit Card (ICC) PIN Encipherment Public Key Exponent
    0x9F2F, // Integrated Circuit Card (ICC) PIN Encipherment Public Key Remainder
    0x5F30, // Service Code
    0x9F32, // Issuer Public Key Exponent
    0x9F33, // Terminal Capabilities
    0x5F34, // Application Primary Account Number (PAN)
    0x9F35, // Terminal Type
    0x5F36, // Transaction Currency Exponent
    0x9F37, // Unpredictable Number
    0x9F38, // Processing Options Data Object List (PDOL)
    0x9F34, // Cardholder Verification Method (CVM) Results
    0x9F3A, // Amount, Reference Currency
    0x9F3B, // Application Reference Currency
    0x9F3C, // Transaction Reference Currency Code
    0x9F3D, // Transaction Reference Currency Exponent
    0x9F40, // Additional Terminal Capabilities
    0x9F41, // Transaction Sequence Counter
    0x42, // Issuer Identification Number (IIN)
    0x9F43, // Application Reference Currency Exponent
    0x9F44, // Application Currency Exponent
    0x9F2D, // Integrated Circuit Card (ICC) PIN Encipherment Public Key Certificate
    0x9F46, // Integrated Circuit Card (ICC) Public Key Certificate
    0x9F47, // Integrated Circuit Card (ICC) Public Key Exponent
    0x9F48, // Integrated Circuit Card (ICC) Public Key Remainder
    0x9F49, // Dynamic Data Authentication Data Object List (DDOL)
    0x9F4A, // Static Data Authentication Tag List
    0x9F4B, // Signed Dynamic Application Data
    0x9F4C, // ICC Dynamic Number
    0x9F4D, // Log Entry
    0x9F4E, // Merchant Name and Location
    0x4F, // Application Identifier (AID)
    0x50, // Application Label
    0x9F51, // Application Currency Code
    0x9F52, // Card Verification Results (CVR)
    0x5F53, // International Bank Account Number (IBAN)
    0x5F54, // Bank Identifier Code (BIC)
    0x5F55, // Issuer Country Code (alpha2 format)
    0x5F56, // Issuer Country Code (alpha3 format)
    0x57, // Track 2 Equivalent Data
    0x9F58, // Lower Consecutive Offline Limit (Card Check)
    0x9F59, // Upper Consecutive Offline Limit (Card Check)
    0x5A, // Application Primary Account Number (PAN)
    0x9F5C, // Cumulative Total Transaction Amount Upper Limit
    0x9F72, // Consecutive Transaction Limit (International - Country)
    0x61, // Application Template
    0x9F65, // Track 2 Bit Map for CVC3
    0x9F66, // Track 2 Bit Map for UN and ATC
    0x9F68, // Mag Stripe CVM List
    0x9F69, // Unpredictable Number Data Object List (UDOL)
    0x9F6B, // Track 2 Data
    0x9F6C, // Mag Stripe Application Version Number (Card)
    0x9F6E, // Unknown Tag
    0x6F, // File Control Information (FCI) Template
    0x70, // EMV Proprietary Template
    0x71, // Issuer Script Template 1
    0x72, // Issuer Script Template 2
    0x73, // Directory Discretionary Template
    0x9F74, // VLP Issuer Authorization Code
    0x9F75, // Cumulative Total Transaction Amount Limit - Dual Currency
    0x9F76, // Secondary Application Currency Code
    0x77, // Response Message Template Format 2
    0x9F7D, // Unknown Tag
    0x9F7F, // Card Production Life Cycle (CPLC) History File Identifiers
    0x80, // Response Message Template Format 1
    0x81, // Amount, Authorised (Binary)
    0x82, // Application Interchange Profile
    0x83, // Command Template
    0x84, // Dedicated File (DF) Name
    0x86, // Issuer Script Command
    0x87, // Application Priority Indicator
    0x88, // Short File Identifier (SFI)
    0x89, // Authorisation Code
    0x8A, // Authorisation Response Code
    0x8C, // Card Risk Management Data Object List 1 (CDOL1)
    0x8D, // Card Risk Management Data Object List 2 (CDOL2)
    0x8E, // Cardholder Verification Method (CVM) List
    0x8F, // Certification Authority Public Key Index
    0x90, // Issuer Public Key Certificate
    0x91, // Issuer Authentication Data
    0x92, // Issuer Public Key Remainder
    0x93, // Signed Static Application Data
    0x94, // Application File Locator (AFL)
    0x95, // Terminal Verification Results
    0x97, // Transaction Certificate Data Object List (TDOL)
    0x98, // Transaction Certificate (TC) Hash Value
    0x99, // Transaction Personal Identification Number (PIN) Data
    0x9A, // Transaction Date
    0x9B, // Transaction Status Information
    0x9C, // Transaction Type
    0x9D, // Directory Definition File (DDF) Name
    0x9F45, // Data Authentication Code
    0xA5, // File Control Information (FCI) Proprietary Template
    0x9F57, // Issuer Country Code
    0x9F39, // Point-of-Service (POS) Entry Mode
    0x9F73, // Currency Conversion Factor
    0x9F42, // Application Currency Code
    0x9F56, // Issuer Authentication Indicator
    0x9F20, // Track 2 Discretionary Data
    0xDF01, // Reference PIN
    0x9F36, // Application Transaction Counter (ATC)
    0x9F4F, // Log Format
    0x5F50, // Issuer URL
    0x9F5A, // Issuer URL2
    0x9F53, // Consecutive Transaction Limit (International)
    0x9F54, // Cumulative Total Transaction Amount Limit
    0x9F55, // Geographic Indicator
};

static void tlv_box_release_tlv(value_t value)
{
    tlv_base_t *tlv = (tlv_base_t *)value.value;
    platform_memory_free(tlv->value);
    platform_memory_free(tlv);
}

tlv_box_t *tlv_box_create(void)
{
    tlv_box_t* box = (tlv_box_t*)platform_memory_alloc(sizeof(tlv_box_t));
	if (box != NULL)
	{
		box->m_list = key_list_create(tlv_box_release_tlv);
		if (box->m_list == NULL)
		{
			return NULL;
		}
		box->m_serialized_buffer = NULL;
		box->m_serialized_bytes = 0;
		box->m_tags_buffer = NULL;
		box->m_tags_number = 0;
		return box;
	}
	else
	{
		return NULL;
	}
}

tlv_error_t tlv_box_destroy(tlv_box_t *box)
{
    if (box == NULL) 
	{
        return BUF_IS_NULL;
    }

    key_list_destroy(box->m_list);
    if (box->m_serialized_buffer != NULL) 
	{
        platform_memory_free(box->m_serialized_buffer);        
    }

    platform_memory_free(box);

    return NO_ERROR;
}

tlv_uint8_t *tlv_box_get_buffer(tlv_box_t *box)
{
    if (box == NULL) 
	{
        return NULL;
    }
    return box->m_serialized_buffer;
}

tlv_int32_t tlv_box_get_size(tlv_box_t *box)
{
    if (box == NULL) 
	{
        return -1;
    }
    return box->m_serialized_bytes;
}

// Each node in the TLV list is obtained by malloc and must be released free after being used. 
// The data of tlv_base_t in the node is also obtained by malloc and needs to be released. So is value in TLv_base_t
tlv_error_t tlv_set_value(tlv_box_t *box, tlv_uint16_t tag, void *value, tlv_uint16_t length)
{
    if (box == NULL || value == NULL) 
	{
        return BUF_IS_NULL;
    }
	
    tlv_base_t *tlv = (tlv_base_t *)platform_memory_alloc(sizeof(tlv_base_t));
    if (tlv == NULL)
    {
        return ALLOC_MEMORY_ERROR;
    }

    tlv->tag = tag;
    tlv->length = length;
    tlv->value = (tlv_uint8_t *) platform_memory_alloc(length);
    if (tlv->value != NULL)
    {
        memcpy(tlv->value, value, length);
    }
    else
    {
        platform_memory_free(tlv);
        return ALLOC_MEMORY_ERROR;
    }

    value_t object;
    object.value = tlv;

    if (key_list_add(box->m_list, tag, object) != TLV_TRUE) 
    {
        platform_memory_free(tlv->value);
        platform_memory_free(tlv);
        return LIST_OPERATION_ERR;
    }    
    box->m_serialized_bytes += sizeof(tlv_uint16_t) * 2 + length;
    
    return NO_ERROR;
}

tlv_error_t tlv_parse(tlv_box_t *box, tlv_uint8_t *buffer, tlv_uint32_t buffersize, tlv_uint8_t byteorder)
{
    tlv_uint32_t offset = 0;
    tlv_uint16_t length = 0, tag = 0;
    tlv_uint32_t i = 0;
   
    if (buffer == NULL) 
	{
        return BUF_IS_NULL;
    }
	box->m_parse_positon = 0;
    while (offset < buffersize) 
	{
        // check input buffer size, prevent cross-referencing of data
        if (offset + sizeof(tlv_uint16_t) <= buffersize) 
		{
            if (byteorder == LITTLE_ENDIAN_PLATFORM) 
			{
                tag = *(buffer + offset) + (*(buffer + offset+1) << 8);
            }
            else 
			{
                tag = *(buffer + offset+1) + (*(buffer + offset) << 8);
            }
            // Iterate through the tags array to see if the tag is valid
            for (i = 0; i < box->m_tags_number; i++) 
			{
                if (box->m_tags_buffer[i] == tag) 
				{
                    break;
                }
            }
            // Get tag
            if (i < box->m_tags_number) 
			{
                offset += sizeof(tlv_uint16_t);
                // check input buffer size, prevent cross-referencing of data
                if (offset + sizeof(tlv_uint16_t) > buffersize) 
				{
                    return LEN_IS_INSUFF;
                }
                if (byteorder == LITTLE_ENDIAN_PLATFORM) 
				{
                    length = *(buffer + offset) + (*(buffer + offset+1) << 8);
                }
                else 
				{
                    length = *(buffer + offset+1) + (*(buffer + offset) << 8);
                }
                offset += sizeof(tlv_uint16_t);
                // check input buffer size, prevent cross-referencing of data
                if (offset + length > buffersize) 
				{
                    return VAL_IS_INSUFF;
                }

                tlv_set_value(box, tag, buffer+offset, length);
                offset += length;
                box->m_parse_positon += (length+4);
            }
            else 
			{
                box->m_parse_positon = buffersize;
                return TAG_BAD_VAL;
            }
        }
        else 
		{
            return TAG_IS_INSUFF;
        }
    }

    return NO_ERROR;
}


tlv_error_t tlv_box_serialize(tlv_box_t *box, tlv_uint8_t byteorder)
{
    if (box == NULL) 
	{
        return BUF_IS_NULL;
    }

    tlv_uint32_t offset = 0;
    tlv_uint16_t tmp1= 0, tmp2=0;
    tlv_uint8_t * buffer = (tlv_uint8_t *) platform_memory_alloc(box->m_serialized_bytes); 
	if (buffer == NULL)
	{
		return ALLOC_MEMORY_ERROR;
	}

    key_list_node_t* node = box->m_list->header;
    while(node != NULL)
    {
        tlv_base_t *tlv = (tlv_base_t *)node->value.value;      

        if (byteorder == LITTLE_ENDIAN_PLATFORM) 
		{
            tmp1 = tlv->tag;
            tmp2 = tlv->length;
        }
        else 
		{
            tmp1 = BSWAP_16(tlv->tag);
            tmp2 = BSWAP_16(tlv->length);
        }
        // check write buffer size, prevent cross-referencing of data
        if (offset+2*sizeof(tlv_uint16_t)+tlv->length <= box->m_serialized_bytes) 
		{
            memcpy(buffer+offset, &tmp1, sizeof(tlv_uint16_t));
            offset += sizeof(tlv_uint16_t);        

            memcpy(buffer+offset, &tmp2, sizeof(tlv_uint16_t));
            offset += sizeof(tlv_uint16_t);

            memcpy(buffer+offset, tlv->value, tlv->length);        
            offset += tlv->length;
        }
        else 
		{
            return CROSS_REF_DATA;
        }

        node = node->next;
    }

    box->m_serialized_buffer = buffer;

    return NO_ERROR;
}

tlv_base_t *tlv_box_get_value(tlv_box_t *box)
{
    value_t object;

    if (box == NULL) 
	{
        return NULL;
    }

    if (box->m_list->header && key_list_get(box->m_list, box->m_list->header->key, &object) != TLV_TRUE)
	{
        return NULL;
    }

    return (tlv_base_t *)object.value;
}

void tlv_box_dest_value(tlv_box_t *box)
{
    key_list_delete(box->m_list, box->m_list->header->key);
}

tlv_int16_t tlv_box_get_length(tlv_box_t *box, tlv_uint16_t tag)
{
    tlv_base_t *tlv_data;
    value_t object;

    if (box == NULL) 
	{
        return -1;
    }
    if (key_list_get(box->m_list, tag, &object) != TLV_TRUE) 
	{
        return -1;
    }
    tlv_data = (tlv_base_t *)object.value;
    return tlv_data->length;
}

tlv_error_t tlv_set_tags(tlv_box_t *box, tlv_uint16_t * tags_buff, tlv_uint16_t tags_num) 
{
    if ((tags_buff == NULL) || (tags_num == 0)) 
	{
        return BUF_IS_NULL;
    }
    box->m_tags_buffer = tags_buff;
    box->m_tags_number = tags_num;
	
	return NO_ERROR;
}
