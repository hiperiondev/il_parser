/**
 * @file literals.c
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

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>

#include "IL.h"
#include "literals.h"
#include "strings.h"

const char *IEC_DATAFORMAT_PFX[] = {
    "2#",            // 0
    "8#",            //
    "16#",           //
    "T#",            //
    "D#",            //
    "TOD#",          //
    "TIME_OF_DAY#",  //
    "DT#",           //
    "DATE_AND_TIME#",//
	"PHY#"           // 9
};

const uint8_t IEC_LITERAL_FORMAT[]= {
    LIT_BASE2,
    LIT_BASE8,
    LIT_BASE16,
    LIT_DURATION,
    LIT_DATE,
    LIT_TIME_OF_DAY,
    LIT_TIME_OF_DAY,
    LIT_DATE_AND_TIME,
    LIT_DATE_AND_TIME,
	LIT_PHY,
	LIT_OTHER
};

const char *IEC_IECTYPE_PFX[] = {
    "?#",       // 0
    "BOOL#",    //
    "SINT#",    //
    "USINT#",   //
    "BYTE#",    //
    "INT#",     //
    "UINT#",    //
    "WORD#",    //
    "DINT#",    //
    "UDINT#",   //
    "DWORD#",   //
    "LINT#",    //
    "ULINT#",   //
    "LWORD#",   //
    "REAL#",    //
    "LREAL#",   //
    "TIME#",    //
    "DATE#",    //
    "TOD#",     //
    "DT#",      //
    "CHAR#",    //
    "WCHAR#",   //
    "STRING#",  //
    "WSTRING#", //
    "POINTER#", //
    "TABLE#",   //
    "USER#",    //
	"R_EDGE#",  //
	"F_EDGE#",  //
	"TIMER#",   //
	"VAR#",     //
	"PHY#"      // 31
};

const char phy_prefix_c[] = {
    'I', //
    'Q', //
    'M'  //
};

const char phy_data_type_c[] = {
    'X', //
    'B', //
    'W', //
    'D'
};

void identify_literal(char *str, uint8_t *datatype, uint8_t *dataformat, char **value) {
	uint8_t n;
	char *ln = NULL;

	ln = trim(str);
	toUpperCase(ln);
	// replace boolean type
	replacestr(ln, "TRUE", "1");
	replacestr(ln, "FALSE", "0");

	//replace others
	replacestr(ln, "%", "PHY#");
    replacestr(ln, "TIME_OF_DAY#", "TOD#");
    replacestr(ln, "DATE_AND_TIME#", "DT#");
	replacestr(ln, "TIME#", "T#");
	replacestr(ln, "DATE#", "D#");

	// delete _
	ln = strremove(ln, "_");

	*dataformat = LIT_OTHER;

	// search typed literal
	*datatype = 0;
	for (n = 1; n < 32; n++) {
		if (strstr(ln, IEC_IECTYPE_PFX[n]) == ln) {
			*datatype = n;
			break;
		}
	}

	if (*datatype != 0)
		memmove(ln, strchr(ln, '#') + 1, strlen(strchr(ln, '#')) + 1);

	if (*datatype == 1) { // is bool
		*dataformat = LIT_BOOLEAN;
		goto end;
	}

	if (*datatype == 31) { // is phy
		*datatype = 0;
		*dataformat = LIT_PHY;
		goto end;
	}

	if (*datatype == IEC_T_TOD) {
        *dataformat = LIT_TIME_OF_DAY;
        goto end;
    }

	if (*datatype == IEC_T_DT) {
        *dataformat = LIT_DATE_AND_TIME;
        goto end;
    }

	// search data prefix
	uint8_t dformat = LIT_NONE;
	for (n = 0; n < 9; n++) {
		if (strstr(ln, IEC_DATAFORMAT_PFX[n]) == ln) {
			dformat = n;
			break;
		}
	}

	if (dformat != LIT_NONE) {
		memmove(ln, strchr(ln, '#') + 1, strlen(strchr(ln, '#')) + 1);
		*dataformat = IEC_LITERAL_FORMAT[dformat];
		goto end;
	}

	if(dformat == LIT_DURATION) {
	    *dataformat = LIT_DURATION;
	    *datatype = IEC_T_TIME;
	    goto end;
	}

	if (!strcmp(str, "0") || !strcmp(str, "1")) {
		*dataformat = LIT_BOOLEAN;
		goto end;
	}

	if (strisinteger(ln)) {
		*dataformat = LIT_INTEGER;
		goto end;
	}

	if (strisfloat(ln)) {
		*dataformat = LIT_REAL;
		goto end;
    }

    if (strisrealexp(ln)) {
        *dataformat = LIT_REAL_EXP;
        goto end;
    }

	ln = str;

	end:
	*value = calloc(strlen(ln) + 1, sizeof(char));
	memcpy(*value, ln, strlen(ln));
}

int parse_phy(char *str, uint8_t *prefix, uint8_t *datatype, uint32_t *phy_a, uint32_t *phy_b) {
    int n, t = 1;
    char *left, *right;
    long int phy_v;

    *prefix = PHY_NONE;
    for (n = 0; n <= PHY_M; n++) {
        if (str[0] == phy_prefix_c[n]) {
            *prefix = n;
            break;
        }

    }
    if (*prefix == PHY_NONE)
        return -2;

    *datatype = PHY_BIT;
    for (n = 0; n <= PHY_DOUBLE; n++) {
        if (str[1] == phy_data_type_c[n]) {
            *datatype = n;
            t++;
            break;
        }
    }

    memmove(str, str + t, strlen(str) - t + 1);

    if (*datatype == PHY_BIT) {
        if (!strisfloat(str))
            return -3;

        split2(str, '.', &left, &right);

        n = str2int(&phy_v, left, 10);
        if (n != STR2INT_SUCCESS || phy_v < 0)
            return -4;
        *phy_a = phy_v;

        n = str2int(&phy_v, right, 10);
        if (n != STR2INT_SUCCESS || phy_v < 0)
            return -5;
        *phy_b = phy_v;

        return 0;
    }

    n = str2int(&phy_v, str, 10);
    if (n != STR2INT_SUCCESS || phy_v < 0)
        return -6;
    *phy_a = phy_v;

    return 0;
}

int parse_time_duration(char *str, uint8_t *hour, uint8_t *min, uint8_t *sec, uint8_t *msec) {
    int n, h = 0, m = 0, s = 0, ms = 0, err;
    long int res;
    double integ;
    char *left, *right;

    replacestr(str, "MS", "L ");
    replacestr(str, "S", "S ");
    replacestr(str, "M", "M ");
    replacestr(str, "H", "H ");

    split2(str, ' ', &left, &right);
    while ((n = strlen(left)) > 0) {
        switch (left[n - 1]) {
            case 'H':
                replacestr(left, "H", "");
                err = str2int(&res, left, 10);
                if (err != STR2INT_SUCCESS)
                    return -1;
                h += res;
                break;

            case 'M':
                replacestr(left, "M", "");
                err = str2int(&res, left, 10);
                if (err != STR2INT_SUCCESS)
                    return -1;
                m += res;
                break;
            case 'S':
                replacestr(left, "S", "");
                err = str2int(&res, left, 10);
                if (err != STR2INT_SUCCESS)
                    return -1;
                s += res;
                break;
            case 'L':
                replacestr(left, "L", "");
                err = str2int(&res, left, 10);
                if (err != STR2INT_SUCCESS)
                    return -1;
                ms += res;
                break;
            default:
                return -1;
        }
        split2(right, ' ', &left, &right);
    }

    modf(ms / 1000, &integ);
    ms = ms % 1000;
    s += integ;

    modf(s / 60, &integ);
    s = s % 60;
    m += integ;

    modf(m / 60, &integ);
    m = m % 60;
    h += integ;

    if (h > 254)
        return -1;

    *hour = h;
    *min = m;
    *sec = s;
    *msec = ms;

    return 0;
}

int parse_time_of_day(char *str, uint8_t *hour, uint8_t *min, uint8_t *sec, uint8_t *msec) {
    int h = 0, m = 0, s = 0, ms = 0, err;
    long int res;
    char *right;

    if (charOccurrence(str, ':') != 2)
        return -1;

    split2(str, ':', &str, &right);
    err = str2int(&res, str, 10);
    if (err != STR2INT_SUCCESS)
        return -2;
    h = res;

    memmove(str, right, strlen(right) + 1);
    split2(str, ':', &str, &right);
    err = str2int(&res, str, 10);
    if (err != STR2INT_SUCCESS)
        return -3;
    m = res;

    memmove(str, right, strlen(right) + 1);
    split2(str, '.', &str, &right);
    err = str2int(&res, str, 10);
    if (err != STR2INT_SUCCESS)
        return -4;
    s = res;
    if (right != NULL) {
        err = str2int(&res, right, 10);
        if (err != STR2INT_SUCCESS)
            return -5;
        ms = res;
    }

    if (h > 254 || m > 59 || s > 59 || ms > 999)
        return -6;

    *hour = h;
    *min = m;
    *sec = s;
    *msec = ms;

    return 0;
}

int parse_calendar_date(char *str, uint8_t *day, uint8_t *month, uint16_t *year) {
    int err;
    long int res;
    char *right;

    if (charOccurrence(str, '-') != 2)
        return -1;

    split2(str, '-', &str, &right);
    err = str2int(&res, str, 10);
    if (err != STR2INT_SUCCESS)
        return -2;
    *year = res;

    memmove(str, right, strlen(right) + 1);
    split2(str, '-', &str, &right);
    err = str2int(&res, str, 10);
    if (err != STR2INT_SUCCESS)
        return -3;
    *month = res;

    err = str2int(&res, right, 10);
    if (err != STR2INT_SUCCESS)
        return -4;
    *day = res;

    return 0;
}

int parse_date_and_time(char *str, uint8_t *day, uint8_t *month, uint16_t *year, uint8_t *hour, uint8_t *min, uint8_t *sec, uint8_t *msec) {
    int cnt = 0, err;
    char *right;
    if (charOccurrence(str, '-') != 3 && charOccurrence(str, ':') != 2)
        return -1;

    for (int n = 0; n < strlen(str); n++) {
        if (str[n] == '-')
            cnt++;
        if (cnt == 3) {
            str[n] = '@';
            break;
        }
    }
    split2(str, '@', &str, &right);
    err = parse_calendar_date(str, day, month, year);
    if (err != 0)
        return -1;

    err = parse_time_of_day(right, hour, min, sec, msec);
    if (err != 0)
        return -2;

    return 0;
}

int parse_cal(char *str) {
    printf("CAL: %s\n", str);
    return 0;
}
