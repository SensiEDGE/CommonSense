/*
   at_base64lib.cpp and at_base64lib.h

   Copyright (C) 2004-2008 René Nyffenegger

   This source code is provided 'as-is', without any express or implied
   warranty. In no event will the author be held liable for any damages
   arising from the use of this software.

   Permission is granted to anyone to use this software for any purpose,
   including commercial applications, and to alter it and redistribute it
   freely, subject to the following restrictions:

   1. The origin of this source code must not be misrepresented; you must not
      claim that you wrote the original source code. If you use this source code
      in a product, an acknowledgment in the product documentation would be
      appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
      misrepresented as being the original source code.

   3. This notice may not be removed or altered from any source distribution.

   René Nyffenegger rene.nyffenegger@adp-gmbh.ch

*/

/* Include ----------------------------------------------------------------- */
#include "at_base64_lib.h"


/**
 * @brief Base64 encode and write to output buffer, errors on buffer overflow
 * 
 * @param input 
 * @param input_size 
 * @param output 
 * @param output_size 
 * @return int number of bytes in output buffer, negative if error occured
 */
int base64_encode_buffer(const char *input, size_t input_size, char *output, size_t output_size)
{
    size_t output_size_check = floor(input_size / 3 * 4);
    size_t mod = input_size % 3;
    output_size_check += mod;

    if (output_size < output_size_check) {
        return -10;
    }

    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    size_t output_ix = 0;

    while (input_size--) {
        char_array_3[i++] = *(input++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; (i < 4) ; i++) {
                output[output_ix++] = base64_chars[char_array_4[i]];
            }
            i = 0;
        }
    }

    if (i) {
        for(j = i; j < 3; j++) {
            char_array_3[j] = '\0';
        }

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++) {
            output[output_ix++] = base64_chars[char_array_4[j]];
        }

        while((i++ < 3)) {
            output[output_ix++] = '=';
        }
    }

    return output_ix;
}