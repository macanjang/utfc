
//NOTE: this converter is currently valid only for mashines with native little-endian byte order  


#ifndef _UTFC__H__
#define _UTFC__H__

//type definitions, may be changed for your system 
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int  uint32_t;

//functions return codes
#define RET_OK 				0 				//success
#define RET_INVALID_UTF16	1				//invalid utf16 atom
#define RET_INVALID_UTF8	2				//invalid utf8  atom
#define RET_BUFFER_OVERFLOW 3				//allocated buffer is too small
#define RET_STRING_TOO_BIG  4				//provided utf16 string is too big

//some macros
#define be_to_le(x) ( ( (x & 0x00FF) << 8 ) | ((x & 0xFF00 ) >> 8 ) ) //for 2 bytes words only
#define LEAD_SURROGATE_MIN 0xD800
#define LEAD_SURROGATE_MAX 0xDBFF
#define TRAIL_SURROGATE_MIN 0xDC00
#define TRAIL_SURROGATE_MAX 0xDFFF

#define LEAD_OFFSET (LEAD_SURROGATE_MIN - ( 0x10000 >> 10 ))
#define SURROGAT_OFFSET ( 0x10000 - (0xD800 << 10) - 0xDC00 )

#define lead_surrogate( x ) ( LEAD_OFFSET + ( x >> 10 ) )
#define trail_surrogate( x ) ( TRAIL_SURROGATE_MIN + ( x & 0x3FF) )

#define UTF16_EOL	'\00'
#define UTF8_EOL 	'\0'

//some function for internal use
int is_valid_utf16_atom( const uint16_t * chr );
int is_valid_utf8_atom( const uint8_t * chr );
uint32_t utf16_code_point( const uint16_t * chr );
unsigned int convert_utf16atom( uint32_t codepoint, uint8_t * dest, const unsigned int dest_size, unsigned int * cur_pos);

//converts little-endian or big-endian utf16 string to utf8
//input:  source    - pointer to utf16be encoded string
//        dest      - pointer to allocated buffer 
//		  dest_size - size of allocated buffer, in bytes
//output: RET_OK if no error occurs. bytes_used - number of bytes used in dest after operation.
//        if an error occurs, value of bytes_used is undefined. 
unsigned int utf16_to_utf8( const uint16_t * source, uint8_t * dest, const unsigned int dest_size, unsigned int * bytes_used, const char to_be );

//converts utf8 string to utf16le or utf16be
//input:  source    - pointer to utf8 encoded string
//        dest      - pointer to allocated buffer 
//		  dest_size - size of allocated buffer, in bytes
//output: RET_OK if no error occurs. bytes_used - number of bytes used in dest after operation.
//        if an error occurs, value of bytes_used is undefined. 
unsigned int utf8_to_utf16( const uint8_t * source, uint16_t * dest, unsigned int dest_size, unsigned int * bytes_used, const char to_be ); 

//user functions - call it from your code

#define utf16be_to_utf8( x, y, z, w ) utf16_to_utf8( x, y, z, w, 42 )

#define utf16le_to_utf8( x, y, z, w ) utf16_to_utf8( x, y, z, w, 0 )

#define utf8_to_utf16le( x, y, z, w ) utf8_to_utf16( x, y, z, w, 0 )

#define utf8_to_utf16be( x, y, z, w ) utf8_to_utf16( x, y, z, w, 21 ) 


#endif