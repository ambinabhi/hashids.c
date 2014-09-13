#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Hashids {

    char* _alphabet = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890';
    char* _seps = 'cfhistuCFHISTU';
    unsigned long int _min_length = 0;
    unsigned long int _max_int_value = 1000000000;

    char* _salt;
    char* _guards;

    int _alphabet_length = 62;
    int _salt_length;

}

char* _str_unique( char* str ) {

    if( str == NULL ) {
        return NULL;
    }

    char* result = ( char* ) malloc( strlen( str ) * sizeof( char ) );
    strcpy( result, str );
    int len = strlen( result );

    int bin_hash[ len ] = {0};
    int ip_index = 0, res_index = 0;
    char temp;

    /* In place removal of duplicate characters*/
    while( *( result + ip_index ) ) {
        temp = *( result + ip_index );
        if( bin_hash[ temp ] == 0 ) {
            bin_hash[ temp ] = 1;
            *( result + res_index ) = *( result + ip_index );
            res_index ++;
        }
        ip_index ++;
    }

    /* After above step string is stringiittg.
    Removing extra iittg after string*/
    *( result + res_index ) = '\0';
    return result;
}

char* _str_intersect( char* str1, char* str2 ) {
    int i = 0, j = 0, k = 0;
    int m = strlen( str1 );
    int n = strlen( str2 );
    char* result = ( char* ) malloc( ( min( m, n ) * sizeof( char ) ) + 1 );
    while( i < m && j < n ) {
        if( str1[ i ] < str2[ j ] ) {
            i ++;
        } else if( str2[ j ] < str1[ i ] ) {
            j ++;
        } else {
            /* if str1[ i ] == str2[ j ] */
            // printf(" %d ", arr2[j++]);
            result[ k++ ] = arr2[ j++ ];
            i++;
        }
    }
    result[ k++ ] = '\0';
    return result;
}

char* _str_diff( char* str1, char* str2 ) {
    int m = strlen( str1 );
    int n = strlen( str2 );
    int j = 0;
    char* result = ( char* ) malloc( ( m * sizeof( char ) ) + 1 );
    for( int i = 0; i < m; i++ ) {
        if( strstr( str2, str1[ i ] ) == NULL ) {
            result[ j++ ] = str1[ i ];
        }
    }
    result[ j++ ] = '\0';
    return result;
}

char* _consistent_shuffle( char* alphabet, char* salt ) {
    if( ! strlen( salt ) ) {
        return alphabet;
    }

    for( int i = strlen( alphabet ) - 1, int v = 0, int p = 0; i > 0; i--, v++ ) {
        int _int;
        char temp;
        v %= strlen( salt );
        p += _int = ( int ) salt[ v ];
        j = ( _int + v + p ) % i;

        temp = alphabet[ j ];
        alphabet[ j ] = alphabet[ i ];
        alphabet[ i ] = temp;
    }

    return alphabet;
}

char* _substr( char* input, int offset, int len ) {
    int inputLength = strlen( input );
    if( offset <= inputLength ) {
        size_t resultLength = inputLength-offset;
        char* result = NULL;
        if( len < resultLength ) {
            resultLength = len;
        }
        result = malloc( resultLength+1 );
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
