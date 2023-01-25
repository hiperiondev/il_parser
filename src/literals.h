/**
 * @file literals.h
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

extern const char *IEC_DATAFORMAT_PFX[];
extern const uint8_t IEC_LITERAL_FORMAT[];
extern const char *IEC_IECTYPE_PFX[];
extern const char phy_prefix_c[];
extern const char phy_data_type_c[];

void identify_literal(char *str, uint8_t *datatype, uint8_t *dataformat, char **value);
 int parse_phy(char *str, uint8_t *prefix, uint8_t *datatype, uint32_t *phy_a, uint32_t *phy_b);
 int parse_time_duration(char *str, uint8_t *hour, uint8_t *min, uint8_t *sec, uint8_t *msec);
 int parse_time_of_day(char *str, uint8_t *hour, uint8_t *min, uint8_t *sec, uint8_t *msec);
 int parse_calendar_date(char *str, uint8_t *day, uint8_t *month, uint16_t *year);
 int parse_date_and_time(char *str, uint8_t *day, uint8_t *month, uint16_t *year, uint8_t *hour, uint8_t *min, uint8_t *sec, uint8_t *msec);
 int parse_cal(char *str);

#endif /* LITERALS_H_ */
