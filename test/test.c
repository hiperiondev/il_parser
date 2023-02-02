/**
 * @file test.c
 * @brief
 * @copyright 2022 Emiliano Augusto Gonzalez (hiperiondev). This project is released under MIT license. Contact: egonzalez.hiperion@gmail.com
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
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "il_parser.h"
#include "internal_parser.h"

/////////////

#define PRINT_PARSED_LINE(line)                                                         \
            char addr[2048], temp_string[2048]="";                                      \
            sprintf(addr, "%u", line.data.jmp_addr);                                    \
            if(line.code != IL_JMP) {                                                   \
                if (line.dataformat == LIT_PHY)                                        \
                    if(line.data.phy.datatype == PHY_BIT) {                             \
                        sprintf(temp_string, "%%%c%c%u.%u",                             \
                            phy_prefix_c[line.data.phy.prefix],                         \
                            phy_data_type_c[line.data.phy.datatype],                    \
                            line.data.phy.phy_a,                                        \
                            line.data.phy.phy_b                                         \
                        );                                                              \
                    } else {                                                            \
                    	sprintf(temp_string, "%%%c%c%u",                                \
                    	    phy_prefix_c[line.data.phy.prefix],                         \
                    	    phy_data_type_c[line.data.phy.datatype],                    \
                    	    line.data.phy.phy_a                                         \
                    	);                                                              \
                    }                                                                   \
                else                                                                    \
                    if(line.str != NULL)                                                \
                        sprintf(temp_string, "%s", line.str);                           \
            }                                                                           \
            printf("  [%04d] %s%s%s%s %s (type: %s, format: %s, value: ",               \
                    pos + 1,                                                            \
					il_commands_str[line.code],                                         \
                    line.c ? "C" : "",                                                  \
                    line.n ? "N" : "",                                                  \
                    line.p ? "(" : "",                                                  \
                    line.code == IL_JMP ? addr : temp_string,                           \
                    pfx_iectype[line.datatype], lit_dataformat_str[line.dataformat]   \
            );                                                                          \
            switch(line.dataformat) {                                                  \
                case LIT_PHY:                                                           \
                      printf("%c, %c, %d, %d)\n",                                       \
                          phy_prefix_c[line.data.phy.prefix],                           \
                          phy_data_type_c[line.data.phy.datatype],                      \
                          line.data.phy.phy_a,                                          \
                          line.data.phy.phy_b                                           \
                      );                                                                \
                      break;                                                            \
                case LIT_DURATION:                                                      \
                      printf("%d, %d, %d, %d)\n",                                       \
                          line.data.dt.tod.hour,                                        \
                          line.data.dt.tod.min,                                         \
                          line.data.dt.tod.sec,                                         \
                          line.data.dt.tod.msec                                         \
                      );                                                                \
                      break;                                                            \
                case LIT_DATE:                                                          \
                      printf("%d, %d, %d)\n",                                           \
                          line.data.dt.date.year,                                       \
                          line.data.dt.date.month,                                      \
                          line.data.dt.date.day                                         \
                      );                                                                \
                      break;                                                            \
                case LIT_TIME_OF_DAY:                                                   \
                      printf("%d, %d, %d, %d)\n",                                       \
                          line.data.dt.tod.hour,                                        \
                          line.data.dt.tod.min,                                         \
                          line.data.dt.tod.sec,                                         \
                          line.data.dt.tod.msec                                         \
                      );                                                                \
                      break;                                                            \
                case LIT_DATE_AND_TIME:                                                 \
                      printf("%d, %d, %d, %d, %d, %d, %d)\n",                           \
                          line.data.dt.date.year,                                       \
                          line.data.dt.date.month,                                      \
                          line.data.dt.date.day,                                        \
                          line.data.dt.tod.hour,                                        \
                          line.data.dt.tod.min,                                         \
                          line.data.dt.tod.sec,                                         \
                          line.data.dt.tod.msec                                         \
                      );                                                                \
                      break;                                                            \
                case LIT_INTEGER:                                                       \
                      printf("%ld)\n",                                                  \
                          line.data.integer                                             \
                      );                                                                \
                      break;                                                            \
                case LIT_REAL:                                                          \
                case LIT_REAL_EXP:                                                      \
                      printf("%f)\n", line.data.real);                                  \
                      break;                                                            \
                case LIT_BOOLEAN:                                                       \
                case LIT_BASE2:                                                         \
                case LIT_BASE8:                                                         \
                case LIT_BASE16:                                                        \
                      printf("%zu)\n", line.data.uinteger);                             \
                      break;                                                            \
                case LIT_CAL:                                                           \
                      printf("[function])\n" );                                         \
                      printf("         [fn: %s]\n", line.data.cal.func);                \
                      for(int n = 0; n<line.data.cal.len; n++) {                        \
                          printf("             [var: %s type: %s, format: %s, value: ", \
                              line.data.cal.var[n],                                     \
							  pfx_iectype[line.data.cal.value[n].datatype],            \
							  lit_dataformat_str[line.data.cal.value[n].dataformat]    \
                          );                                                            \
                          PRINT_VALUE(line.data.cal.value[n]);                          \
                          printf("]    \n");                                            \
                      }                                                                 \
                      break;                                                            \
                case LIT_STRING:                                                        \
                case LIT_VAR:                                                           \
                      printf("%s)\n", temp_string);                                     \
                      break;                                                            \
                 default:                                                               \
                      printf("[null])\n");                                              \
             }

#define PRINT_VALUE(line)                                          \
            switch(line.dataformat) {                             \
                case LIT_PHY:                                      \
                      printf("%c, %c, %d, %d",                     \
                          phy_prefix_c[line.data.phy.prefix],      \
                          phy_data_type_c[line.data.phy.datatype], \
                          line.data.phy.phy_a,                     \
                          line.data.phy.phy_b                      \
                      );                                           \
                      break;                                       \
                case LIT_DURATION:                                 \
                      printf("%d, %d, %d, %d",                     \
                          line.data.dt.tod.hour,                   \
                          line.data.dt.tod.min,                    \
                          line.data.dt.tod.sec,                    \
                          line.data.dt.tod.msec                    \
                      );                                           \
                      break;                                       \
                case LIT_DATE:                                     \
                      printf("%d, %d, %d",                         \
                          line.data.dt.date.year,                  \
                          line.data.dt.date.month,                 \
                          line.data.dt.date.day                    \
                      );                                           \
                      break;                                       \
                case LIT_TIME_OF_DAY:                              \
                      printf("%d, %d, %d, %d",                     \
                          line.data.dt.tod.hour,                   \
                          line.data.dt.tod.min,                    \
                          line.data.dt.tod.sec,                    \
                          line.data.dt.tod.msec                    \
                      );                                           \
                      break;                                       \
                case LIT_DATE_AND_TIME:                            \
                      printf("%d, %d, %d, %d, %d, %d, %d",         \
                          line.data.dt.date.year,                  \
                          line.data.dt.date.month,                 \
                          line.data.dt.date.day,                   \
                          line.data.dt.tod.hour,                   \
                          line.data.dt.tod.min,                    \
                          line.data.dt.tod.sec,                    \
                          line.data.dt.tod.msec                    \
                      );                                           \
                      break;                                       \
                case LIT_INTEGER:                                  \
                      printf("%ld",                                \
                          line.data.integer                        \
                      );                                           \
                      break;                                       \
                case LIT_REAL:                                     \
                case LIT_REAL_EXP:                                 \
                      printf("%f", line.data.real);                \
                      break;                                       \
                case LIT_BOOLEAN:                                  \
                case LIT_BASE2:                                    \
                case LIT_BASE8:                                    \
                case LIT_BASE16:                                   \
                      printf("%zu", line.data.uinteger);           \
                      break;                                       \
                case LIT_STRING:                                   \
                case LIT_VAR:                                      \
                      printf("%s", line.str);                      \
                      break;                                       \
                 default:                                          \
                      printf("\n");                                \
             }

int main(void) {
    parsed_il_t parsed;
    int lines;
    int pos;
    char *dest = NULL;

    printf("--------------------------------------------\n< START test1 >\n\n");
    lines = compile_il("test.il", &parsed);

    printf("\n----------\n- RESULT (%d)-\n----------\n", lines);

    for (pos = 0; pos < lines; pos++) {
        PRINT_PARSED_LINE(parsed.result[pos]);
    }

    free_il(&parsed);
    printf("\n< END test1 >\n--------------------------------------------\n\n");

    /////////////////////////////////////////////////////////////////////////////////

    printf("\n\n--------------------------------------------\n< START test2 >\n\n");
    lines = compile_il("test2.il", &parsed);

    printf("\n----------\n- RESULT (%d)-\n----------\n", lines);

    for (pos = 0; pos < lines; pos++) {
        PRINT_PARSED_LINE(parsed.result[pos]);
    }

    printf("\n----------\n- JSON -\n----------\n");

    parsed2json(parsed, &dest);
    printf("\n%s\n", dest);

    free(dest);
    free_il(&parsed);
    printf("\n< END test2 >\n--------------------------------------------\n\n");

    return EXIT_SUCCESS;
}
