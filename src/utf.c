#include "utf/utf.h"

#include <stdio.h>

int is_valid_utf16_atom( const uint16_t * chr )
{
	//TODO
	return 1 == 1;
}


unsigned int utf8_to_utf16( const uint8_t * source, uint16_t * dest, unsigned int dest_size )
{
	//TODO
	return 0;
}

unsigned int utf16le_to_utf8( const uint16_t * source, uint8_t * dest, const unsigned int dest_size, unsigned int * bytes_used )
{
	unsigned int cur_pos = 0;
	unsigned int offset = 0;
	while ( 42 )
	{
		if ( !is_valid_utf16_atom( source+offset ) )
			return RET_INVALID_UTF16; 
		if ( source[offset] == '\00' )
			break;
		if ( source[offset] < 0x80 && cur_pos < dest_size ) 
			dest[cur_pos++] = source[offset];
		else
		if ( source[offset] < 0x800 && cur_pos + 1 < dest_size  )
		{
			dest[cur_pos++] = (uint8_t)((source[offset] >> 6)            | 0xc0);
            dest[cur_pos++] = (uint8_t)((source[offset] & 0x3f)          | 0x80);
		}
		else 
		if ( source[offset] < 0x10000 && cur_pos + 2 < dest_size ) 
		{              
            dest[cur_pos++] = (uint8_t)((source[offset] >> 12)           | 0xe0);
            dest[cur_pos++] = (uint8_t)(((source[offset] >> 6) & 0x3f)   | 0x80);
            dest[cur_pos++] = (uint8_t)((source[offset] & 0x3f)          | 0x80);
        }
        else 
        if ( cur_pos + 3 < dest_size )
        {      
            dest[cur_pos++] = (uint8_t)((source[offset] >> 18)           | 0xf0);
            dest[cur_pos++] = (uint8_t)(((source[offset] >> 12) & 0x3f)  | 0x80);
            dest[cur_pos++] = (uint8_t)(((source[offset] >> 6) & 0x3f)   | 0x80);
            dest[cur_pos++] = (uint8_t)((source[offset] & 0x3f)          | 0x80);
        }
        else 
        	return RET_BUFFER_OVERFLOW;
        offset++;
	}
	dest[cur_pos] = 0x0000;
	*bytes_used = cur_pos; 
	return RET_OK;	
}

unsigned int utf16be_to_utf8( const uint16_t * source, uint8_t * dest, const unsigned int dest_size, unsigned int * bytes_used )
{
	unsigned int cur_pos = 0;
	unsigned int offset = 0;
	while ( 42 )
	{
		if ( !is_valid_utf16_atom( source+offset ) )
			return RET_INVALID_UTF16; 
		if ( source[offset] == '\00' )
			break;
		if ( be_to_le(source[offset]) < 0x80 && cur_pos < dest_size ) 										//one byte 
			dest[cur_pos++] = be_to_le(source[offset]);
		else
		if ( be_to_le(source[offset]) < 0x800 && cur_pos + 1 < dest_size  ) 									//two bytes
		{
			dest[cur_pos++] = (uint8_t)((be_to_le(source[offset]) >> 6)            | 0xc0);
            dest[cur_pos++] = (uint8_t)((be_to_le(source[offset]) & 0x3f)          | 0x80);
           
		}
		else 
		if ( be_to_le(source[offset]) < 0x10000 && cur_pos + 2 < dest_size) 
		{              																// three bytes
            dest[cur_pos++] = (uint8_t)((be_to_le(source[offset]) >> 12)           | 0xe0);
            dest[cur_pos++] = (uint8_t)(((be_to_le(source[offset] >> 6)) & 0x3f)   | 0x80);
            dest[cur_pos++] = (uint8_t)((be_to_le(source[offset] & 0x3f))          | 0x80);
        
        }
        else 
        if ( cur_pos + 3 < dest_size ) 
        {      																	    // four bytes
            dest[cur_pos++] = (uint8_t)((be_to_le(source[offset]) >> 18)           | 0xf0);
            dest[cur_pos++] = (uint8_t)(((be_to_le(source[offset]) >> 12) & 0x3f)  | 0x80);
            dest[cur_pos++] = (uint8_t)(((be_to_le(source[offset]) >> 6) & 0x3f)   | 0x80);
            dest[cur_pos++] = (uint8_t)((be_to_le(source[offset]) & 0x3f)          | 0x80);
        } 
        else 
        	return RET_BUFFER_OVERFLOW;
        offset++;
        
	}
	dest[cur_pos] = '\00';
	*bytes_used = cur_pos; 
	return RET_OK;	
}