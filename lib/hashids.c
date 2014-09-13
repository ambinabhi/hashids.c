/*
Hashids
http://hashids.org/c
@author Udit Desai <desaiuditd@gmail.com>
https://github.com/desaiuditd/hashids.c
hashids may be freely distributed under the GPL v2 license.
*/

#include "hashids.h"

#include "e4c.h"

#define VERSION 0.2

/* internal settings */
#define MIN_ALPHABET_LENGTH 16
#define SEP_DIV 3.5
#define GUARD_DIV 12

/* error messages */
#define E_ALPHABET_LENGTH 'alphabet must contain at least %d unique characters'
#define E_ALPHABET_SPACE 'alphabet cannot contain spaces'

struct Hashids Hashids( char* salt, unsigned long int min_length, char* alphabet ) {

	struct Hashids hash_obj;

	/* handle parameters */

	hash_obj._salt = salt;
	hash_obj._salt_length = strlen( salt );

	if( ( int ) min_length > 0 ) {
		hash_obj._min_length = ( int ) min_length;
	}

	if( alphabet ) {
		hash_obj._alphabet = _str_unique( alphabet );
		hash_obj._alphabet_length = strlen( hash_obj._alphabet );
	}

	if( strlen( hash_obj._alphabet ) < MIN_ALPHABET_LENGTH ) {
		throw (RuntimeException, sprintf( E_ALPHABET_LENGTH, MIN_ALPHABET_LENGTH ) );
	}

	if( strstr( hash_obj._alphabet, ' ' ) != NULL ) {
		throw ( RuntimeException, self::E_ALPHABET_SPACE );
	}

	char* temp_alphabet = ( char* ) malloc( strlen( hash_obj._alphabet ) * sizeof( char ) );
	char* temp_seps = ( char* ) malloc( strlen( hash_obj._seps ) * sizeof( char ) );
	strcpy( temp_alphabet, hash_obj._alphabet );
	strcpy( temp_seps, hash_obj._seps );

	hash_obj._seps = _str_intersect( temp_alphabet, temp_seps );
	hash_obj._alphabet = _str_diff( temp_alphabet, temp_seps );
	hash_obj._seps = _consistent_shuffle( hash_obj._seps, hash_obj._salt );

	if( hash_obj._seps != NULL || ( strlen( hash_obj._alphabet ) / strlen( hash_obj._seps ) ) > SEP_DIV ) {

		int  seps_length, diff;
		seps_length = ( int ) ceil( strlen( hash_obj._alphabet ) / SEP_DIV );

		if( seps_length == 1 ) {
			seps_length++;
		}

		if( seps_length > strlen( hash_obj._seps ) ) {
			diff = seps_length - strlen( hash_obj._seps );
			hash_obj._seps .= _substr( hash_obj._alphabet, 0, diff );
			hash_obj._alphabet = _substr( hash_obj._alphabet, diff );
		} else {
			hash_obj._seps = _substr( hash_obj._seps, 0, seps_length );
		}
	}

	hash_obj._alphabet = _consistent_shuffle( hash_obj._alphabet, hash_obj._salt );
	int guard_count = ( int ) ceil( strlen( hash_obj._alphabet ) / GUARD_DIV );

	if( strlen( hash_obj._alphabet ) < 3 ) {
		hash_obj._guards = _substr( hash_obj._seps, 0, guard_count );
		hash_obj._seps = _substr( hash_obj._seps, guard_count );
	} else {
		hash_obj._guards = _substr( hash_obj._alphabet, 0, guard_count );
		hash_obj._alphabet = _substr( hash_obj._alphabet, guard_count );
	}

	return hash_obj;
}

int main() {

}
