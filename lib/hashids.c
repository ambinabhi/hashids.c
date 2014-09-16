/*
Hashids
http://hashids.org/c
@author Udit Desai <desaiuditd@gmail.com>
https://github.com/desaiuditd/hashids.c
hashids may be freely distributed under the GPL v2 license.
*/

#include "hashids.h"

#include "e4c.h"

#define VERSION 0.3

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
		hash_obj._alphabet_length = _str_unique( alphabet );
		strcpy( hash_obj._alphabet, alphabet );
	}

	if( strlen( hash_obj._alphabet ) < MIN_ALPHABET_LENGTH ) {
		throw (RuntimeException, sprintf( E_ALPHABET_LENGTH, MIN_ALPHABET_LENGTH ) );
	}

	if( strstr( hash_obj._alphabet, ' ' ) != NULL ) {
		throw ( RuntimeException, self::E_ALPHABET_SPACE );
	}

	char* temp_alphabet = ( char* ) malloc( sizeof( char ) );
	char* temp_seps = ( char* ) malloc( strlen( sizeof( char ) );
	strcpy( temp_alphabet, hash_obj._alphabet );
	strcpy( temp_seps, hash_obj._seps );

	_str_intersect( temp_alphabet, temp_seps, hash_obj._seps );
	_str_diff( temp_alphabet, temp_seps, hash_obj._alphabet );
	_consistent_shuffle( hash_obj._seps, hash_obj._salt );

	if( hash_obj._seps != NULL || ( strlen( hash_obj._alphabet ) / strlen( hash_obj._seps ) ) > SEP_DIV ) {

		int  seps_length, diff;
		seps_length = ( int ) ceil( strlen( hash_obj._alphabet ) / SEP_DIV );

		if( seps_length == 1 ) {
			seps_length++;
		}

		if( seps_length > strlen( hash_obj._seps ) ) {
			diff = seps_length - strlen( hash_obj._seps );
			_substr( hash_obj._alphabet, 0, diff, hash_obj._seps );
			_substr( hash_obj._alphabet, diff, strlen( hash_obj._alphabet ) - diff, hash_obj._alphabet );
		} else {
			_substr( hash_obj._seps, 0, seps_length, hash_obj._seps );
		}
	}

	_consistent_shuffle( hash_obj._alphabet, hash_obj._salt );
	int guard_count = ( int ) ceil( strlen( hash_obj._alphabet ) / GUARD_DIV );

	if( strlen( hash_obj._alphabet ) < 3 ) {
		_substr( hash_obj._seps, 0, guard_count, hash_obj._guards );
		_substr( hash_obj._seps, guard_count, strlen( hash_obj._seps ) - guard_count, hash_obj._seps );
	} else {
		_substr( hash_obj._alphabet, 0, guard_count, hash_obj._guards );
		_substr( hash_obj._alphabet, guard_count, strlen( hash_obj._alphabet ) - guard_count, hash_obj._alphabet );
	}

	return hash_obj;
}

int encode( Hashids hash_obj, char* encoded_str, long int numbers, ... ) {
	va_list valist;
	va_start( valist, numbers );
	if( ! numbers ) {
		va_end( valist );
		return '';
	}
	int* numbers_array = ( int* ) malloc( numbers * sizeof( int ) );
	int j = 0;
	for( int i = 0; i < numbers; i++ ) {
		int number = va_arg( valist, int );
		if( number < 0 || number > hash_obj._max_int_value ) {
			va_end( valist );
			return '';
		} else {
			numbers_array[ j++ ] = number;
		}
	}
	va_end( valist );
	_encode( hash_obj, numbers_array, j, encoded_str );

	return strlen( encoded_str );
}

int main() {

}
