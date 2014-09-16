#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>

struct Hashids {

    int _alphabet_length = 62;

    char _alphabet[ _alphabet_length ] = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890';
    char _seps[ 14 ] = 'cfhistuCFHISTU';

    unsigned long int _min_length = 0;
    unsigned long int _max_int_value = 1000000000;
    int _salt_length;

    char* _salt;
    char* _guards;
}

int _str_unique( char* str ) {

    if( str == NULL ) {
        return 0;
    }

    int len = strlen( str );

    int bin_hash[ len ] = {0};
    int ip_index = 0, res_index = 0;
    char temp;

    /* In place removal of duplicate characters*/
    while( *( str + ip_index ) ) {
        temp = *( str + ip_index );
        if( bin_hash[ temp ] == 0 ) {
            bin_hash[ temp ] = 1;
            *( str + res_index ) = *( str + ip_index );
            res_index ++;
        }
        ip_index ++;
    }

    /* After above step string is stringiittg.
    Removing extra iittg after string*/
    *( str + res_index ) = '\0';
    return strlen( str );
}

int _str_intersect( char* str1, char* str2, char* intersection ) {
    int i = 0, j = 0;
    int m = strlen( str1 );
    int n = strlen( str2 );

    free( intersection );
    intersection = ( char* ) malloc( sizeof( char ) );
    while( i < m && j < n ) {
        if( str1[ i ] < str2[ j ] ) {
            i ++;
        } else if( str2[ j ] < str1[ i ] ) {
            j ++;
        } else {
            /* if str1[ i ] == str2[ j ] */
            strcat( intersection, arr2[ j++ ] );
            i++;
        }
    }
    strcat( intersection, '\0' );
    return strlen( intersection );
}

int _str_diff( char* str1, char* str2, char* diff ) {
    int m = strlen( str1 );
    int n = strlen( str2 );
    free( diff );
    diff = ( char* ) malloc( sizeof( char ) );
    for( int i = 0; i < m; i++ ) {
        if( strstr( str2, str1[ i ] ) == NULL ) {
            strcat( diff, str1[ i ] );
        }
    }
    strcat( diff, '\0' );
    return strlen( diff );
}

int _consistent_shuffle( char* alphabet, char* salt ) {
    if( alphabet == NULL || salt == NULL ) {
        return 0;
    }

    if( ! strlen( salt ) ) {
        return strlen( alphabet );
    }

    for( int i = strlen( alphabet ) - 1, int v = 0, int p = 0; i > 0; i--, v++ ) {
        int _int;
        char temp;
        v %= strlen( salt );
        // TODO: Check for atoi - in php it was used ord.
        p += _int = atoi( salt[ v ] );
        j = ( _int + v + p ) % i;

        temp = alphabet[ j ];
        alphabet[ j ] = alphabet[ i ];
        alphabet[ i ] = temp;
    }

    return strlen( alphabet );
}

int _substr( char* input, int offset, int len, char* result ) {
    int inputLength = strlen( input );
    if( offset <= inputLength ) {
        int resultLength = inputLength-offset;
        free( result );
        if( len < resultLength ) {
            resultLength = len;
        }
        result = ( char* ) malloc( sizeof( char ) );
        if( NULL != result ) {
            strncpy( result, input+offset, resultLength );
            result[ resultLength ] = '\0';
        }
        return strlen( result );
    } else {
        /* Offset is larger than the string length */
        return 0;
    }
}

int _hash( int input, char* alphabet, char* hash ) {
    free( hash );
    hash = ( char* ) malloc( sizeof( char ) );
    strcpy( hash, '' );
    alphabet_length = strlen( alphabet );
    do {
        strcpy( hash, strcat( alphabet[ input % alphabet_length ], hash ) );
		input = ( int ) ( input / alphabet_length );
	} while ( input );
    return strlen( hash );
}

int _encode( Hashids hash_obj, int* numbers, int numbers_size, char* encoded_str ) {
    char* alphabet = ( char* ) malloc( sizeof( char ) );
    strcpy( alphabet, hash_obj._alphabet );
	int numbers_hash_int = 0;

    for( int i = 0; i < numbers_size; i++ ) {
        numbers_hash_int += ( numbers[ i ] % ( i + 100 ) );
    }

    char* lottery;
    lottery = ( char* ) malloc( sizeof( char ) );
    encoded_str = ( char* ) malloc( sizeof( char ) );
    strcpy( lottery, alphabet[ numbers_hash_int % strlen( alphabet ) ] );
    strcpy( encoded_str, alphabet[ numbers_hash_int % strlen( alphabet ) ] );

    for( int i=0 ; i < numbers_size ; i++ ) {
        char* temp_substr;
        _substr( strcat( strcat( lottery, hash_obj._salt ), alphabet ), 0, strlen( alphabet ), temp_substr );
        _consistent_shuffle( alphabet, temp_substr );

        char* temp;
        int temp_len = _hash( numbers[i], alphabet, temp );
        char* last = ( char* ) malloc( sizeof( char ) );
        strcpy( last, temp );
        strcat( encoded_str, temp );

        if( i + 1 < numbers_size ) {
            // TODO: Check for atoi - in php it was used ord.
            number[ i ] %= ( atoi( last ) + i );
            int seps_index = numbers[ i ] % strlen( hash_obj._seps );
            strcat( encoded_str, hash_obj._seps[ seps_index ] );
        }
    }

    if( strlen( encoded_str ) < hash_obj._min_hash_length ) {
        int guard_index = ( numbers_hash_int + atoi( encoded_str[ 0 ] ) ) % strlen( hash_obj._guards );

        char* guard = ( char* ) malloc( sizeof( char ) );
        strcpy( guard, hash_obj._guards[ guard_index ] );

        strcpy( encoded_str, strcat( guard, encoded_str ) )

        if( strlen( encoded_str ) < hash_obj._min_hash_length ) {
            guard_index = ( numbers_hash_int + atoi( encoded_str[ 2 ] ) ) % strlen( hash_obj._guards );

            strcpy( guard, hash_obj._guards[ guard_index ] );

            strcat( encoded_str, guard );
        }
    }

    int half_length = ( int ) ( strlen( alphabet ) / 2 );
	while( strlen( encoded_str ) < hash_obj._min_hash_length ) {
		_consistent_shuffle( alphabet, alphabet );

        char* temp_substr;
        _substr( alphabet, half_length, strlen( alphabet ) - half_length, temp_substr );
        strcpy( encoded_str, temp_substr );

        _substr( alphabet, half_length, strlen( alphabet ) - half_length, temp_substr );
        strcat( encoded_str, strcat( temp_substr, encoded_str ) );
        _substr( alphabet, 0, half_length, temp_substr );
        strcat( encoded_str, temp_substr );

		int excess = strlen( encoded_str ) - hash_obj._min_hash_length;
		if( excess > 0 ) {
            _substr( encoded_str, excess / 2, hash_obj._min_hash_length, temp_substr );
            strcpy( encoded_str, temp_substr );
		}
	}

    return strlen( encoded_str );
}
