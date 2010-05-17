#include "utf/utf.h"

#include <stdio.h>
#include <limits.h>

int is_valid_utf16_atom( const uint16_t * chr )
{
	//http://unicode.org/faq/utf_bom.html#utf16-7 
	//if ( *chr == 0xFFFF || *chr == 0xFFFE || ( *chr >= 0xFDD0 && *chr <= 0xFDEF ) )  //this chars are not printable, but valid
	//	return 1 == 2;
	if ( ( *chr >= LEAD_SURROGATE_MIN  && *chr <= LEAD_SURROGATE_MAX ) && ( *(chr+1) < TRAIL_SURROGATE_MIN  || *(chr+1) > TRAIL_SURROGATE_MAX ) )
		return 1 == 2;
	
	if ( ( *chr >= TRAIL_SURROGATE_MIN && *chr <= TRAIL_SURROGATE_MAX ) && ( *(chr-1) < LEAD_SURROGATE_MIN  || *(chr-1) > LEAD_SURROGATE_MAX ) )
		return 1 == 2;
	
	return 1 == 1;
}


uint32_t utf16le_code_point( const uint16_t * chr )
{
	if ( *chr >= LEAD_SURROGATE_MIN ) //we have 2 word value
	{
		return ( chr[0] << 10 ) + chr[1] + SURROGAT_OFFSET;
	}
	else
		return (uint32_t)(*chr);
}

uint32_t utf16be_code_point( const uint16_t * chr )
{
	if ( be_to_le(*chr) >= LEAD_SURROGATE_MIN ) //we have 2 word value
	{
		return ( be_to_le(chr[0]) << 10 ) + be_to_le(chr[1]) + SURROGAT_OFFSET;
	}
	else
		return (uint32_t)(be_to_le(*chr));	
}


unsigned int utf8_to_utf16( const uint8_t * source, uint16_t * dest, unsigned int dest_size, const char to_be )
{
	//TODO
	unsigned int offset = 0;
	while ( *(source + offset) != UTF8_EOL )
	{
			
	}
	return 0;
}


unsigned int utf16_to_utf8( const uint16_t * source, uint8_t * dest, const unsigned int dest_size, unsigned int * bytes_used, const char to_be )
{
	unsigned int cur_pos = 0;
	unsigned int offset = 0;
	uint32_t cp;
	int i = 0;
	
	while ( 42 )
	{
		if ( !is_valid_utf16_atom( source+offset ) )
			return RET_INVALID_UTF16; 
		if ( source[offset] == UTF16_EOL ) 
			break;
		
		if ( offset == UINT_MAX )
			return RET_STRING_TOO_BIG; // omg, what was it?
			
		if ( to_be == 0 )
			cp = utf16le_code_point( &source[offset++] );
		else 
			cp = utf16be_code_point( &source[offset++] );
		printf( "code point %X \n", cp );
		if ( cp > 0xFFFF )
			offset++;
		
		unsigned int rez = convert_utf16atom( cp, dest, dest_size, &cur_pos );
		if ( rez != RET_OK )
			return rez; 
	   
	}
	dest[cur_pos++] = UTF8_EOL;
	*bytes_used = cur_pos; 
	return RET_OK;	
}


// internal function. supposes, that codepoint is valid utf16 value. do not call directly from your code
// 
unsigned int convert_utf16atom( uint32_t codepoint, uint8_t * dest, const unsigned int dest_size, unsigned int * cur_pos)
{
	
	if ( codepoint < 0x80 && *cur_pos < dest_size ) 
		dest[(*cur_pos)++] = codepoint;
	else
	if ( codepoint < 0x800 && *cur_pos + 1 < dest_size  )
	{
		dest[(*cur_pos)++] = (uint8_t)(( codepoint >> 6 )            | 0xc0);
        dest[(*cur_pos)++] = (uint8_t)(( codepoint & 0x3f )          | 0x80);
	}
	else 
	if ( codepoint < 0x10000 && *cur_pos + 2 < dest_size ) 
	{              
    	dest[(*cur_pos)++] = (uint8_t)((  codepoint >> 12 )           | 0xe0);
        dest[(*cur_pos)++] = (uint8_t)((( codepoint >> 6 ) & 0x3f)    | 0x80);
        dest[(*cur_pos)++] = (uint8_t)((  codepoint & 0x3f )          | 0x80);
     }
     else
     if ( *cur_pos + 3 < dest_size )
     {      
     	dest[(*cur_pos)++] = (uint8_t)((  codepoint >> 18)           | 0xf0);
        dest[(*cur_pos)++] = (uint8_t)((( codepoint >> 12) & 0x3f)   | 0x80);
        dest[(*cur_pos)++] = (uint8_t)((( codepoint >> 6) & 0x3f)    | 0x80);
        dest[(*cur_pos)++] = (uint8_t)((  codepoint & 0x3f)          | 0x80);
     }
     else 
     	return RET_BUFFER_OVERFLOW;
	return RET_OK;
} 


