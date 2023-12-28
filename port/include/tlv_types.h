
#ifndef __TLV_PORT_TYPES_H__
#define __TLV_PORT_TYPES_H__

typedef char                tlv_chr_t;
typedef char*               tlv_str_t;
typedef signed char         tlv_int8_t;
typedef unsigned char       tlv_uint8_t;
typedef signed short        tlv_int16_t;
typedef unsigned short      tlv_uint16_t;
typedef signed int          tlv_int32_t;
typedef unsigned int        tlv_uint32_t;
typedef signed long long    tlv_int64_t;
typedef unsigned long long  tlv_uint64_t;
typedef float               tlv_float_t;
typedef double              tlv_double_t;
typedef unsigned char       tlv_bool_t;

enum {
    TLV_FALSE = 0,
    TLV_TRUE = 1
};

#endif
