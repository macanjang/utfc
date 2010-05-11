#ifndef _UTFC__H__
#define _UTFC__H__

//type definitions, may be changed for your system 
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

//functions return codes
#define RET_OK 				0 				//success
#define RET_INVALID_UTF16	1				//invalid utf16 atom
#define RET_INVALID_UTF8	2				//invalid utf8  atom
#define RET_BUFFER_OVERFLOW 3				//allocated buffer is too small

//some macros
#define be_to_le(x) ( ( (x & 0x00FF) << 8 ) | ((x & 0xFF00 ) >> 8 ) )

//user functions - call it from your code

//converts utf8 string to utf16le
//TODO
unsigned int utf8_to_utf16le( const uint8_t * source, uint16_t * dest, unsigned int dest_size); 

//converts big-endian utf16 string to utf8
//input:  source    - pointer to utf16be encoded string
//        dest      - pointer to allocated buffer 
//		  dest_size - size of allocated buffer, in bytes
//output: RET_OK if no error occurs. bytes_used - number of bytes used in dest after operation.
//        if an error occurs, value of bytes_used is undefined. 
unsigned int utf16be_to_utf8( const uint16_t * source, uint8_t * dest, const unsigned int dest_size, unsigned int * bytes_used);

//converts little-endian utf16 string to utf8
//input:  source    - pointer to utf16be encoded string
//        dest      - pointer to allocated buffer 
//		  dest_size - size of allocated buffer, in bytes
//output: RET_OK if no error occurs. bytes_used - number of bytes used in dest after operation.
//        if an error occurs, value of bytes_used is undefined. 
unsigned int utf16le_to_utf8( const uint16_t * source, uint8_t * dest, const unsigned int dest_size, unsigned int * bytes_used );
 

//some function for internal use
int is_valid_utf16_atom( const uint16_t * chr );
int is_valid_utf8_atom( const uint8_t * chr );


#endif