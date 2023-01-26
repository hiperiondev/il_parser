/**
 * @file parser.h
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

#ifndef LITERALS_H_
#define LITERALS_H_

#include <stdint.h>

#include "IL.h"
typedef struct il_cmd_str il_t;

extern const char *IEC_DATAFORMAT_PFX[];
extern const uint8_t IEC_LITERAL_FORMAT[];
extern const char *IEC_IECTYPE_PFX[];
extern const char phy_prefix_c[];
extern const char phy_data_type_c[];

void identify_literal(il_t *line, char **value);
 int parse_value(il_t *line, int pos);
 int parse_phy(il_t *line);
 int parse_time_duration(il_t *line);
 int parse_time_of_day(il_t *line);
 int parse_calendar_date(il_t *line);
 int parse_date_and_time(il_t *line);
 int parse_cal(il_t *line);

#endif /* LITERALS_H_ */
