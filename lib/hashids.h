/**
* Hashids
* http://hashids.org/c
* @author Udit Desai <desaiuditd@gmail.com>
* https://github.com/desaiuditd/hashids.c
* hashids may be freely distributed under the MIT license.
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>

#define HASHIDS_VERSION 0.4

/* internal settings */
#define MIN_ALPHABET_LENGTH 16
#define SEP_DIV 3.5
#define GUARD_DIV 12

/* error messages */
#define E_ALPHABET_LENGTH 'alphabet must contain at least %d unique characters'
#define E_ALPHABET_SPACE 'alphabet cannot contain spaces'

// Data Structure used for Hashids
struct Hashids {

    int _alphabet_length = 62;

    char _alphabet[] = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890';
    char _seps[] = 'cfhistuCFHISTU';

    unsigned long int _min_length = 0;
    unsigned long int _max_int_value = 1000000000;
    int _salt_length;

    char *_salt;
    char *_guards;
}

// Returns string intersection : string containing all the common characters between str1 and str2.
char *_str_intersect( char *str1, char *str2 ) {
    int i = 0, j = 0;
    int m = strlen( str1 );
    int n = strlen( str2 );

    char *intersection = ( char * ) malloc( sizeof( char ) );
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
    return intersection;
}

// Returns string diff : str1 - str2 i.e., string containing characters which are there in str1 but not in str2.
char *_str_diff( char *str1, char *str2 ) {
    int m = strlen( str1 );
    int n = strlen( str2 );
    char *diff = ( char * ) malloc( sizeof( char ) );
    for( int i = 0; i < m; i++ ) {
        if( strstr( str2, str1[ i ] ) == NULL ) {
            strcat( diff, str1[ i ] );
        }
    }
    strcat( diff, '\0' );
    return diff;
}

// Returns a part of string
char *_substr( char *input, int offset, int len ) {
    int inputLength = strlen( input );
    if( offset <= inputLength ) {
        int resultLength = inputLength-offset;
        if( len < resultLength ) {
            resultLength = len;
        }
        char *result = ( char * ) malloc( sizeof( char ) );
        if( NULL != result ) {
            strncpy( result, input+offset, resultLength );
            result[ resultLength ] = '\0';
        }
        return result;
    } else {
        /* Offset is larger than the string length */
        return NULL;
    }
}

char *_str_replace( char *search , char *replace , char *subject ) {
    char  *p = NULL , *old = NULL , *new_subject = NULL ;
    int c = 0 , search_size;

    search_size = strlen( search );

    //Count how many occurences
    for( p = strstr( subject , search ); p != NULL; p = strstr( p + search_size , search ) ) {
        c++;
    }

    //Final size
    c = ( strlen( replace ) - search_size ) * c + strlen( subject );

    //New subject with new size
    new_subject = malloc( c );

    //Set it to blank
    strcpy( new_subject , "" );

    //The start position
    old = subject;

    for( p = strstr( subject , search ); p != NULL; p = strstr( p + search_size , search ) ) {
        //move ahead and copy some text from original subject , from a certain position
        strncpy( new_subject + strlen( new_subject ) , old , p - old );

        //move ahead and copy the replacement text
        strcpy( new_subject + strlen( new_subject ) , replace );

        //The new start position after this search match
        old = p + search_size;
    }

    //Copy the part after the last search match
    strcpy( new_subject + strlen( new_subject ) , old );

    return new_subject;
}

// Removes trailing/leading white spaces from the string
char *_trim( char *str ) {
    char *ibuf = str, *obuf = str;
    int i = 0, cnt = 0;

    char *return_str;
    strcpy( return_str, str );

    /*
    **  Trap NULL
    */
    if( return_str ) {
        /*
        **  Remove leading spaces (from RMLEAD.C)
        */
        for( ibuf = return_str; *ibuf && isspace( *ibuf ); ++ibuf ) {

        }

        if( return_str != ibuf ) memmove( return_str, ibuf, ibuf - return_str );

        /*
        **  Collapse embedded spaces (from LV1WS.C)
        */
        while( *ibuf ) {
            if( isspace( *ibuf ) && cnt ) {
                ibuf++;
            } else {
                if ( ! isspace( *ibuf ) ) {
                    cnt = 0;
                } else {
                    *ibuf = ' ';
                    cnt = 1;
                }
                obuf[ i++ ] = *ibuf++;
            }
        }
        obuf[ i ] = '\0';

        /*
        **  Remove trailing spaces (from RMTRAIL.C)
        */
        while( --i >= 0 ) {
            if( ! isspace( obuf[ i ] ) ) break;
        }
        obuf[ ++i ] = '\0';
    }
    return return_str;
}

// Explodes a string to array of strings based on delimiter
int _explode( char *delim, char *str, char ***r ) {
    char **res = ( char** ) malloc( sizeof( char* ) * strlen( str ) );
    char *p;
    int i = 0;
    while( p = strtok( str, delim ) ) {
        res[ i ] = malloc( strlen( p ) + 1 );
        strcpy( res[ i ], p );
        ++i;
        str = NULL;
    }
    res = realloc( res, sizeof( char* ) * i );
    *r = res;
    return i;
}

// Hash Shuffle Logic Function
char *_consistent_shuffle( char *alphabet, char *salt ) {
    if( alphabet == NULL || salt == NULL ) {
        return NULL;
    }

    if( ! strlen( salt ) ) {
        return alphabet;
    }

    char *return_str;
    strcpy( return_str, alphabet );

    for( int i = strlen( return_str ) - 1, int v = 0, int p = 0; i > 0; i--, v++ ) {
        int _int;
        char temp;
        v %= strlen( salt );
        // TODO: Check for atoi - in php it was used ord.
        p += _int = atoi( salt[ v ] );
        j = ( _int + v + p ) % i;

        temp = return_str[ j ];
        return_str[ j ] = return_str[ i ];
        return_str[ i ] = temp;
    }

    return return_str;
}

// Hash Function
char *_hash( int input, char *alphabet ) {
    char *hash = ( char * ) malloc( sizeof( char ) );
    strcpy( hash, '' );
    alphabet_length = strlen( alphabet );
    do {
        strcpy( hash, strcat( alphabet[ input % alphabet_length ], hash ) );
        input = ( int ) ( input / alphabet_length );
    } while ( input );
    return hash;
}

// Encode Function
char *_encode( struct Hashids *hash_obj, int *numbers, int numbers_size ) {
    char *alphabet = ( char * ) malloc( sizeof( char ) );
    strcpy( alphabet, hash_obj->_alphabet );
    int numbers_hash_int = 0;
    char *temp;

    for( int i = 0; i < numbers_size; i++ ) {
        numbers_hash_int += ( numbers[ i ] % ( i + 100 ) );
    }

    char *lottery;
    lottery = ( char * ) malloc( sizeof( char ) );
    encoded_str = ( char * ) malloc( sizeof( char ) );
    strcpy( lottery, alphabet[ numbers_hash_int % strlen( alphabet ) ] );
    strcpy( encoded_str, alphabet[ numbers_hash_int % strlen( alphabet ) ] );

    for( int i=0 ; i < numbers_size ; i++ ) {
        temp = alphabet;
        alphabet = _consistent_shuffle( alphabet, _substr( strcat( strcat( lottery, hash_obj->_salt ), alphabet ), 0, strlen( alphabet ) ) );
        free( temp );

        char *last = _hash( numbers[i], alphabet );
        strcat( encoded_str, last );

        if( i + 1 < numbers_size ) {
            // TODO: Check for atoi - in php it was used ord.
            number[ i ] %= ( atoi( last ) + i );
            int seps_index = numbers[ i ] % strlen( hash_obj->_seps );
            strcat( encoded_str, hash_obj->_seps[ seps_index ] );
        }
    }

    if( strlen( encoded_str ) < hash_obj->_min_hash_length ) {
        int guard_index = ( numbers_hash_int + atoi( encoded_str[ 0 ] ) ) % strlen( hash_obj->_guards );

        char *guard = ( char * ) malloc( sizeof( char ) );
        strcpy( guard, hash_obj->_guards[ guard_index ] );

        strcpy( encoded_str, strcat( guard, encoded_str ) )

        if( strlen( encoded_str ) < hash_obj->_min_hash_length ) {
            guard_index = ( numbers_hash_int + atoi( encoded_str[ 2 ] ) ) % strlen( hash_obj->_guards );

            strcpy( guard, hash_obj->_guards[ guard_index ] );

            strcat( encoded_str, guard );
        }
    }

    int half_length = ( int ) ( strlen( alphabet ) / 2 );
    while( strlen( encoded_str ) < hash_obj->_min_hash_length ) {
        temp = alphabet;
        alphabet = _consistent_shuffle( alphabet, alphabet );
        free( temp );

        strcpy( encoded_str, strcat( _substr( alphabet, half_length, strlen( alphabet ) - half_length ), strcat( encoded_str, _substr( alphabet, 0, half_length ) ) ) );

        int excess = strlen( encoded_str ) - hash_obj->_min_hash_length;
        if( excess > 0 ) {
            strcpy( encoded_str, _substr( encoded_str, excess / 2, hash_obj->_min_hash_length ) );
        }
    }

    return encoded_str;
}

// Decode Function
int _decode( struct Hashids *hash_obj, char *hash, char *alphabet, int *numbers ) {
    free( numbers );
    int num_count = 0;
    numbers = ( int * ) malloc( ( num_count + 1 ) * sizeof( int ) );

    char *hash_breakdown = _str_replace( hash_obj->_guards, ' ', hash );
    char **hash_array;
    char *temp;
    int hash_array_len = _explode( ' ', hash_breakdown, &hash_array );

    int i = 0;
    if( hash_array_len == 3 || hash_array_len == 2 ) {
        i = 1;
    }

    strcpy( hash_breakdown, hash_array[ i ] );
    if( hash_breakdown[ 0 ] != NULL ) {

        char lottery = hash_breakdown[ 0 ];

        temp = hash_breakdown;
        hash_breakdown = _substr( hash_breakdown, 1 , strlen( hash_breakdown ) - 1 );
        free( temp );

        temp = hash_breakdown;
        hash_breakdown = _str_replace( hash_obj->_seps, ' ', hash_breakdown );
        free( temp );

        free( hash_array );
        hash_array_len = _explode( ' ', hash_breakdown, &hash_array );

        for( int i = 0; i < hash_array_len; i++ ) {
            temp = alphabet;
            alphabet = _consistent_shuffle( alphabet, _substr( strcat( lottery, strcat( hash_obj->_salt, alphabet ) ), 0, strlen( alphabet ) ) );
            free( temp );

            numbers[ num_count++ ] = ( int ) _unhash( hash_array[ i ], alphabet );
            numbers = ( int * ) realloc( numbers, ( num_count + 1 ) * sizeof( int ) );
        }

        if( strcmp( _encode( hash_obj, numbers, num_count ), hash ) != 0 ) {
            free( numbers_array );
            num_count = 0;
        }
    }

    return num_count;
}

// Hashids main function to retrieve Hash
struct Hashids *Hashids( char *salt, unsigned long int min_length, char *alphabet ) {

    struct Hashids *hash_obj = ( struct hashids * ) malloc( sizeof( struct Hashids ) );

    char *temp;

    /* handle parameters */

    hash_obj->_salt = salt;
    hash_obj->_salt_length = strlen( salt );

    if( ( int ) min_length > 0 ) {
        hash_obj->_min_length = ( int ) min_length;
    }

    strcpy( hash_obj->_alphabet, alphabet );

    int len = strlen( hash_obj->_alphabet );

    char *unique_alphabet = ( char * ) malloc( sizeof( char ) );
    strcpy( unique_alphabet, '' );

    for( int i = 0; i !== len; i++) {
        if( strstr( unique_alphabet, hash_obj->_alphabet[ i ] ) == NULL ) {
            strcat( unique_alphabet, hash_obj->_alphabet[ i ] );
        }
    }

    strcpy( hash_obj._alphabet, unique_alphabet );

    if( strlen( hash_obj->_alphabet ) < MIN_ALPHABET_LENGTH ) {
        printf( E_ALPHABET_LENGTH, MIN_ALPHABET_LENGTH );
        free( hash_obj );
        return;
    }

    if( strstr( hash_obj->_alphabet, ' ' ) != NULL ) {
        printf( E_ALPHABET_SPACE );
        free( hash_obj );
        return;
    }

    /* seps should contain only characters present in alphabet; alphabet should not contains seps */

    char *temp_alphabet = ( char * ) malloc( sizeof( char ) );
    char *temp_seps = ( char * ) malloc( strlen( sizeof( char ) );
    strcpy( temp_alphabet, hash_obj->_alphabet );
    strcpy( temp_seps, hash_obj->_seps );

    temp = hash_obj->_seps;
    hash_obj->_seps = _str_intersect( temp_alphabet, temp_seps );
    free( temp );

    temp = hash_obj->_alphabet;
    hash_obj->_alphabet = _str_diff( temp_alphabet, temp_seps );
    free( temp );

    temp = hash_obj->_alphabet;
    hash_obj->_alphabet = _str_replace( ' ', '', hash_obj->_alphabet );
    free( temp );

    temp = hash_obj->_salt;
    hash_obj->_salt = _str_replace( ' ', '', hash_obj->_salt );
    free( temp );

    temp = hash_obj->_seps;
    hash_obj->_seps = _consistent_shuffle( hash_obj->_seps, hash_obj->_salt );
    free( temp );

    if( hash_obj->_seps == NULL || ! strlen( hash_obj->_seps ) || ( strlen( hash_obj->_alphabet ) / strlen( hash_obj->_seps ) ) > SEP_DIV ) {

        int  seps_length, diff;
        seps_length = ( int ) ceil( strlen( hash_obj->_alphabet ) / SEP_DIV );

        if( seps_length == 1 ) {
            seps_length++;
        }

        if( seps_length > strlen( hash_obj->_seps ) ) {
            diff = seps_length - strlen( hash_obj->_seps );
            strcat( hash_obj->_seps, _substr( hash_obj->_alphabet, 0, diff ) );

            temp = hash_obj->_alphabet;
            hash_obj->_alphabet = _substr( hash_obj->_alphabet, diff, strlen( hash_obj->_alphabet ) - diff );
            free( temp );
        } else {
            temp = hash_obj->_seps;
            hash_obj->_seps = _substr( hash_obj->_seps, 0, seps_length );
            free( temp );
        }
    }

    temp = hash_obj->_alphabet;
    hash_obj->_alphabet = _consistent_shuffle( hash_obj->_alphabet, hash_obj->_salt );
    free( temp );

    int guard_count = ( int ) ceil( strlen( hash_obj->_alphabet ) / GUARD_DIV );

    if( strlen( hash_obj->_alphabet ) < 3 ) {
        temp = hash_obj->_guards;
        hash_obj->_guards = _substr( hash_obj->_seps, 0, guard_count );
        free( temp );

        temp = hash_obj->_seps;
        hash_obj->_seps = _substr( hash_obj->_seps, guard_count, strlen( hash_obj->_seps ) - guard_count );
        free( temp );
    } else {
        temp = hash_obj->_guards;
        hash_obj->_guards = _substr( hash_obj->_alphabet, 0, guard_count );
        free( temp );

        temp = hash_obj->_alphabet;
        hash_obj->_alphabet = _substr( hash_obj->_alphabet, guard_count, strlen( hash_obj->_alphabet ) - guard_count );
        free( temp );
    }

    return hash_obj;
}

char *encode( struct Hashids *hash_obj, long int numbers, ... ) {
    va_list valist;
    va_start( valist, numbers );
    if( ! numbers ) {
        va_end( valist );
        return '';
    }
    int *numbers_array = ( int * ) malloc( numbers * sizeof( int ) );
    int j = 0;
    for( int i = 0; i < numbers; i++ ) {
        int number = va_arg( valist, int );
        if( number < 0 || number > hash_obj->_max_int_value ) {
            va_end( valist );
            return '';
        } else {
            numbers_array[ j++ ] = number;
        }
    }
    va_end( valist );

    return _encode( hash_obj, numbers_array, j );
}

int decode( struct Hashids *hash_obj, char *hash, int *numbers ) {
    if( hash == NULL || ! strlen( hash ) ) {
        free( numbers );
        return 0;
    }

    _trim( hash );
    return _decode( hash_obj, hash, hash_obj->_alphabet, numbers );
}
