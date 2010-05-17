//just for tests
//may not be working or compilable

#include "../src/utf/utf.h"
#include <stdio.h>
#include <wchar.h>


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <malloc.h>

#include <string.h>

#include <limits.h>

int main(int argc, char ** argv)
{ 
	/*FILE * f = fopen("test-utf16.txt", "r");
	if ( !f )
	  return 1;
	struct stat st;
	stat("test-utf16.txt", &st);
	printf("file size: %d bytes \n", st.st_size);
	char * utf16_str = malloc( st.st_size * sizeof(char) );  
	fread(utf16_str, 1, st.st_size, f);
	printf("this is utf16 string: %s \n", utf16_str);
	int i = 0;
	for ( ; i < st.st_size; i++ )
	  printf("%d = %X ", i, utf16_str[i]);
	printf("\n");
	char * utf8_str = malloc( st.st_size * sizeof(char) * 4 );
	//char * utf8_str = malloc( 100 );
	
	//skip 4 first bytes - BOM
	printf("malloc ok\n");
	//utf16_str ; 
	int rez = 0;
	if ( utf16le_to_utf8( (uint16_t*)utf16_str, utf8_str, st.st_size * sizeof(char) * 4, &rez) == RET_OK ) 
		printf("this is converted string: %s, num bytes written: %d, strlen %d \n", utf8_str, rez, strlen(utf8_str));
	else
		printf("func failed\n"); 
	
	fclose(f);
	free(utf8_str);
	//todo - write test cases*/
	
	int i = 0;	
	
	printf("%d\n", sizeof( int ));
	printf("UINT_MAX %X\n", UINT_MAX);
	
	uint8_t  utf16_str_le[] = { 0x34, 0xD8, 0x1E, 0xDD, 0, 0 };
	//uint16_t  utf16_str_le[] = { 0xD834, 0xDD1E, 0 };
	
	uint8_t  utf16_str_be[] = { 0xD8, 0x34, 0xDD, 0x1E, 0, 0 };
	
	
	uint16_t  utf16_str_le1[] = { 0x6C34, 0 };
	uint8_t  utf16_str_be1[] = { 0x6C, 0x34, 0, 0 };

	char * utf8_str = malloc( 5 );
	unsigned int used;
	if  ( utf16le_to_utf8( (uint16_t*)utf16_str_le, utf8_str, 5, &used) != RET_OK )
		printf("failed\n");
	else
		printf("converted string %s, bytes used %d \n", utf8_str, used); 
	
	if ( utf8_to_utf16( utf8_str, utf16_str_le, 5, &used, 0 ) != RET_OK )
		printf("failed\n");
	else
	{
		printf("ok, bytes used %d \n", used);
		for ( ; i < used ; i++ )
		{
			printf("%X ", utf16_str_le[i]);
		}
		printf("\n");
	}
	
	if  ( utf16be_to_utf8( (uint16_t*)utf16_str_be, utf8_str, 5, &used) != RET_OK )
		printf("failed\n");
	else
		printf("converted string %s, bytes used %d \n", utf8_str, used); 
	
	utf8_to_utf16( utf8_str, utf16_str_le, 5, &used, 0 );
	
	
	if  ( utf16le_to_utf8( (uint16_t*)utf16_str_le1, utf8_str, 5, &used) != RET_OK )
		printf("failed\n");
	else
		printf("converted string %s, bytes used %d \n", utf8_str, used); 
	
	if  ( utf16be_to_utf8( (uint16_t*)utf16_str_be1, utf8_str, 5, &used) != RET_OK )
		printf("failed\n");
	else
		printf("converted string %s, bytes used %d \n", utf8_str, used); 
	
	
	if ( utf8_to_utf16( utf8_str, utf16_str_le, 5, &used, 0 ) != RET_OK )
		printf("failed\n");
	else
	{
		printf("ok, bytes used %d \n", used);
		for ( i = 0; i < used ; i++ )
		{
			printf("%X ", utf16_str_le[i]);
		}
		printf("\n");
	}
	free( utf8_str );
	return 0;
}