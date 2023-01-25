/**
 * @file parser.c
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
#include "parser.h"
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

void identify_literal(il_t *line, char **value) {
	uint8_t n;
	char *ln = NULL;

	ln = trim((*line).str);
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

	(*line).data_format = LIT_OTHER;

	// search typed literal
	(*line).data_type = 0;
	for (n = 1; n < 32; n++) {
		if (strstr(ln, IEC_IECTYPE_PFX[n]) == ln) {
		    (*line).data_type = n;
			break;
		}
	}

	if ((*line).data_type != 0)
		memmove(ln, strchr(ln, '#') + 1, strlen(strchr(ln, '#')) + 1);

	if ((*line).data_type == IEC_T_BOOL) { // is bool
	    (*line).data_format = LIT_BOOLEAN;
		goto end;
	}

	if ((*line).data_type == IEC_T_PHY) { // is phy
	    (*line).data_type = 0;
		(*line).data_format = LIT_PHY;
		goto end;
	}

	if ((*line).data_type == IEC_T_TOD) { //is TOD
	    (*line).data_format = LIT_TIME_OF_DAY;
        goto end;
    }

	if ((*line).data_type == IEC_T_DT) { // is DT
	    (*line).data_format = LIT_DATE_AND_TIME;
        goto end;
    }

	if ((ln[0] == '"' && ln[strlen(ln) - 1] == '"') || (ln[0] == '\'' && ln[strlen(ln) - 1] == '\'')) {
	    (*line).data_format = LIT_STRING;
	    ln = (*line).str;
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

	// erase data format prefix
	if (dformat != LIT_NONE) {
		memmove(ln, strchr(ln, '#') + 1, strlen(strchr(ln, '#')) + 1);
		(*line).data_format = IEC_LITERAL_FORMAT[dformat];
		goto end;
	}

	if(dformat == LIT_DURATION) {
	    (*line).data_format = LIT_DURATION;
	    (*line).data_type = IEC_T_TIME;
	    goto end;
	}

	if (!strcmp((*line).str, "0") || !strcmp((*line).str, "1")) {
	    (*line).data_format = LIT_BOOLEAN;
		goto end;
	}

	if (strisinteger(ln)) {
	    (*line).data_format = LIT_INTEGER;
		goto end;
	}

	if (strisfloat(ln)) {
	    (*line).data_format = LIT_REAL;
		goto end;
    }

    if (strisrealexp(ln)) {
        (*line).data_format = LIT_REAL_EXP;
        goto end;
    }

	ln = (*line).str;

	end:

	*value = calloc(strlen(ln) + 1, sizeof(char));
	memcpy(*value, ln, strlen(ln));
}

int parse_phy(il_t *line) {
    int n, t = 1;
    char *left, *right;
    long int phy_v;

    (*line).data.phy.prefix = PHY_NONE;
    for (n = 0; n <= PHY_M; n++) {
        if ((*line).str[0] == phy_prefix_c[n]) {
            (*line).data.phy.prefix = n;
            break;
        }

    }
    if ((*line).data.phy.prefix == PHY_NONE)
        return -2;

    (*line).data_type = PHY_BIT;
    for (n = 0; n <= PHY_DOUBLE; n++) {
        if ((*line).str[1] == phy_data_type_c[n]) {
            (*line).data_type = n;
            t++;
            break;
        }
    }

    memmove((*line).str, (*line).str + t, strlen((*line).str) - t + 1);

    if ((*line).data_type == PHY_BIT) {
        if (!strisfloat((*line).str))
            return -3;

        split2((*line).str, '.', &left, &right);

        n = str2int(&phy_v, left, 10);
        if (n != STR2INT_SUCCESS || phy_v < 0)
            return -4;
        (*line).data.phy.phy_a = phy_v;

        n = str2int(&phy_v, right, 10);
        if (n != STR2INT_SUCCESS || phy_v < 0)
            return -5;
        (*line).data.phy.phy_b = phy_v;

        return 0;
    }

    n = str2int(&phy_v, (*line).str, 10);
    if (n != STR2INT_SUCCESS || phy_v < 0)
        return -6;
    (*line).data.phy.phy_a = phy_v;

    return 0;
}

int parse_time_duration(il_t *line) {
    int n, h = 0, m = 0, s = 0, ms = 0, err;
    long int res;
    double integ;
    char *left, *right;

    replacestr((*line).str, "MS", "L ");
    replacestr((*line).str, "S", "S ");
    replacestr((*line).str, "M", "M ");
    replacestr((*line).str, "H", "H ");

    split2((*line).str, ' ', &left, &right);
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

    (*line).data.dt.tod.hour = h;
    (*line).data.dt.tod.min = m;
    (*line).data.dt.tod.sec = s;
    (*line).data.dt.tod.msec = ms;

    return 0;
}

int parse_time_of_day(il_t *line) {
    int h = 0, m = 0, s = 0, ms = 0, err;
    long int res;
    char *right;

    if (charOccurrence((*line).str, ':') != 2)
        return -1;

    split2((*line).str, ':', &((*line).str), &right);
    err = str2int(&res, (*line).str, 10);
    if (err != STR2INT_SUCCESS)
        return -2;
    h = res;

    memmove((*line).str, right, strlen(right) + 1);
    split2((*line).str, ':', &((*line).str), &right);
    err = str2int(&res, (*line).str, 10);
    if (err != STR2INT_SUCCESS)
        return -3;
    m = res;

    memmove((*line).str, right, strlen(right) + 1);
    split2((*line).str, '.', &((*line).str), &right);
    err = str2int(&res, (*line).str, 10);
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

    (*line).data.dt.tod.hour = h;
    (*line).data.dt.tod.min = m;
    (*line).data.dt.tod.sec = s;
    (*line).data.dt.tod.msec = ms;

    return 0;
}

int parse_calendar_date(il_t *line) {
    int err;
    long int res;
    char *right;

    if (charOccurrence((*line).str, '-') != 2)
        return -1;

    split2((*line).str, '-', &((*line).str), &right);

    err = str2int(&res, (*line).str, 10);
    if (err != STR2INT_SUCCESS)
        return -2;
    (*line).data.dt.date.year = res;

    memmove((*line).str, right, strlen(right) + 1);
    split2((*line).str, '-', &((*line).str), &right);
    err = str2int(&res, (*line).str, 10);
    if (err != STR2INT_SUCCESS)
        return -3;
    (*line).data.dt.date.month = res;

    err = str2int(&res, right, 10);
    if (err != STR2INT_SUCCESS)
        return -4;
    (*line).data.dt.date.day = res;

    return 0;
}

int parse_date_and_time(il_t *line) {
    int cnt = 0, err;
    char *right;
    if (charOccurrence((*line).str, '-') != 3 && charOccurrence((*line).str, ':') != 2)
        return -1;

    for (int n = 0; n < strlen((*line).str); n++) {
        if ((*line).str[n] == '-')
            cnt++;
        if (cnt == 3) {
            (*line).str[n] = '@';
            break;
        }
    }

    split2((*line).str, '@', &((*line).str), &right);
    err = parse_calendar_date(line);
    if (err != 0)
        return -2;


    memmove((*line).str, right, strlen(right) + 1);
    err = parse_time_of_day(line);
    if (err != 0)
        return -3;

    return 0;
}

int parse_cal(il_t *line) {
    //int cnt = 0, err;
    //char *right;
    printf("CAL: %s\n", (*line).str);

    return 0;
}
