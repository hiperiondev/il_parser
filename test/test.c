/**
 * @file test.c
 * @brief
 * @copyright 2023 Emiliano Augusto Gonzalez (hiperiondev). This project is released under MIT license. Contact: egonzalez.hiperion@gmail.com
 * @see Project Site: https://github.com/hiperiondev/il_parser
 * @note This is based on other projects. Please contact their authors for more information.
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "il_parser.h"

int main(void) {
    parsed_il_t parsed;

    printf("------------------ test 1 ------------------\n");
    parse_file_il("test1.il", &parsed);
    printf("[lines = %d]\n", parsed.lines);

    for (int n = 0; n < parsed.lines; n++) {
        free_il(&(parsed.result[n]));
    }
    free(parsed.result);
    printf("--------------------------------------------\n");
    printf("\n");
    printf("------------------ test 2 ------------------\n");
    parse_file_il("test2.il", &parsed);
    printf("[lines = %d]\n", parsed.lines);

    for (int n = 0; n < parsed.lines; n++) {
        free_il(&(parsed.result[n]));
    }
    free(parsed.result);
    printf("--------------------------------------------\n");

    return 0;
}
