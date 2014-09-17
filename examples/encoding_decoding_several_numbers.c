#include <stdio.h>

/* include hashids lib */
#include "../lib/hashids.h"

int main() {

	/* create the class object */
	struct Hashids *hash_obj = Hashids( "this is my salt", 0, "" );

	if( hash_obj == NULL ) {
		return;
	}

	/* encode several numbers into one id */
	char *id = encode( hash_obj, 1337, 5, 77, 12345678 );

	printf( "Encoded Hash : %s\n", id );

	/* decode that id back */
	int *numbers;
	int num_count = decode( hash_obj, id, numbers );

	/* `$numbers` is always an array */
	printf( "Original Numbers : " );
	int i;
	for( i = 0; i < num_count; i++ ) {
		printf("%d ", numbers[ i ] );
	}
	printf( "\n" );
}
