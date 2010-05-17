#include "utf/utf.h"

#include <stdio.h>
#include <limits.h>

int is_valid_utf16_atom( const uint16_t * chr )
{
	//http://unicode.org/faq/utf_bom.html#utf16-7 
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


unsigned int utf8_to_utf16( const uint8_t * source, uint16_t * dest, unsigned int dest_size, unsigned int * bytes_used, const char to_be )
{
        unsigned int offset = 0;
		unsigned int cur_pos = 0;
		uint32_t cp;
		int i = 0;
		while ( *(source + offset) != UTF8_EOL )
        {
			if ( offset == UINT_MAX )
				return RET_STRING_TOO_BIG;
			
			if ( (*(source + offset) & 0xF0) == 0xF0 ) // 4 bytes sequence
			{
				//calc code point from 4 bytes
				if ( ( (*(source + offset + 1) & 0x80 ) != 0x80 ) && ( (*(source + offset + 2) & 0x80 ) != 0x80 ) && ( (*(source + offset + 3) & 0x80 ) != 0x80 ) ) //second or third byte has no marker 
					return RET_INVALID_UTF8;	
				cp = ( ( ( ( ( *(source + offset) & 0x7 ) << 2 ) | ( ( *(source + offset + 1) & 0x30 ) >> 4 ) ) ) << 16 ) | 
						 ( (  ( ( *(source + offset + 1) & 0xF ) << 4 ) | ( ( *(source + offset + 2) & 0x3C ) >> 2 )  ) << 8 )  | 
						 ( ( ( *(source + offset + 2) & 0x3 ) << 6 ) | ( *(source + offset + 3) & 0x3F ) );
				offset+=4;
			} 
			else
			if ( (*(source + offset) & 0xE0) == 0xE0 ) // 3 bytes sequence
			{
				//calc code point from 3 bytes
				if ( ( (*(source + offset + 1) & 0x80 ) != 0x80 ) && ( (*(source + offset + 2) & 0x80 ) != 0x80 ) )  //second or third byte has no marker 
					return RET_INVALID_UTF8;	
				cp = ( ( ( (*( source + offset ) & 0xF) << 4) | ( ( *(source + offset + 1) & 0x3C ) >> 2 ) ) << 8 ) | ( ( *(source + offset + 1) & 0x3 ) << 6 ) | ( *(source + offset + 2) & 0x3F )  ;
				offset+=3;
			}
			else // 2 bytes sequence
			if ( ( *(source + offset ) & 0xC0 ) == 0xC0 )
			{
				//calc code point from 2 bytes	
				if ( (*(source + offset + 1) & 0x80 ) != 0x80 ) //second byte has no marker 
					return RET_INVALID_UTF8;
				cp = (((*(source + offset) & 0x1F ) >> 2 ) << 8 ) | ( ( (*(source + offset )  & 0x3) << 6 ) | ( *(source + offset + 1) & 0x3F ) );
				offset+=2;
				if ( cp <= 127 ) //overlong sequence
					return RET_INVALID_UTF8;
			}
			else // ASCII char?
			if ( *( source + offset ) <= 127 )
			{
				cp = *( source + offset++ );
			} 
			else //second byte?
				return RET_INVALID_UTF8;
			
			//encode codepoint 
			if ( cp > 0xFFFF ) //two words sequence
			{
				//printf("big cp %X\n", cp);
				//TODO: make surrogate pair
				//TODO: le/be
			}
			else 
			{
				dest[cur_pos++] = cp;
				//TODO: le/be
			}
			if ( cur_pos >= dest_size )
				return RET_BUFFER_OVERFLOW;

        }
		*bytes_used = ( cur_pos + 1 ) * 2;
		dest[cur_pos] = UTF16_EOL;
        return RET_OK;
}


unsigned int utf16_to_utf8( const uint16_t * source, uint8_t * dest, const unsigned int dest_size, unsigned int * bytes_used, const char to_be )
{
	unsigned int cur_pos = 0;
	unsigned int offset = 0;
	unsigned int rez = 0;
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
		if ( cp > 0xFFFF )
			offset++;
		
		rez = convert_utf16atom( cp, dest, dest_size, &cur_pos );
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


