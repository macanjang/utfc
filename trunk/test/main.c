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

int main(int argc, char ** argv)
{
	FILE * f = fopen("test-utf16.txt", "r");
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
	//todo - write test cases
	return 0;
}