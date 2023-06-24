/**
 * @file il_parser.c
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
#include <string.h>
#include <ctype.h>
#include <inttypes.h>

#include "il_parser.h"
#include "strings.h"

typedef struct il_str_s {
    const char *str; //
       uint8_t code; //
          bool c;    // conditional
          bool n;    // negate
          bool p;    // push '('
} il_str_t;

typedef struct il_label_s {
    String label; //
    uint32_t line;   //
} il_label_t;

static const il_str_t commands[] = {
//  STR_CMD, CODE  , C, N, P
  { "LD"   , IL_LD , 0, 0, 0 },
  { "LDN"  , IL_LD , 0, 1, 0 },
  { "ST"   , IL_ST , 0, 0, 0 },
  { "STN"  , IL_ST , 0, 1, 0 },
  { "S"    , IL_S  , 0, 0, 0 },
  { "R"    , IL_R  , 0, 0, 0 },
  { "AND"  , IL_AND, 0, 0, 0 },
  { "&"    , IL_AND, 0, 0, 0 },
  { "ANDN" , IL_AND, 0, 1, 0 },
  { "&N"   , IL_AND, 0, 1, 0 },
  { "OR"   , IL_OR , 0, 0, 0 },
  { "ORN"  , IL_OR , 0, 1, 0 },
  { "XOR"  , IL_XOR, 0, 0, 0 },
  { "XORN" , IL_XOR, 0, 1, 0 },
  { "AND(" , IL_AND, 0, 0, 1 },
  { "&("   , IL_AND, 0, 0, 1 },
  { "ANDN(", IL_AND, 0, 1, 1 },
  { "&N("  , IL_AND, 0, 1, 1 },
  { "OR("  , IL_OR , 0, 0, 1 },
  { "ORN(" , IL_OR , 0, 1, 1 },
  { "XOR(" , IL_XOR, 0, 0, 1 },
  { "XORN(", IL_XOR, 0, 1, 1 },
  { "ADD"  , IL_ADD, 0, 0, 0 },
  { "SUB"  , IL_SUB, 0, 0, 0 },
  { "MUL"  , IL_MUL, 0, 0, 0 },
  { "DIV"  , IL_DIV, 0, 0, 0 },
  { "GT"   , IL_GT , 0, 0, 0 },
  { "GE"   , IL_GE , 0, 0, 0 },
  { "EQ"   , IL_EQ , 0, 0, 0 },
  { "NE"   , IL_NE , 0, 0, 0 },
  { "LE"   , IL_LE , 0, 0, 0 },
  { "LT"   , IL_LT , 0, 0, 0 },
  { "ADD(" , IL_ADD, 0, 0, 1 },
  { "SUB(" , IL_SUB, 0, 0, 1 },
  { "MUL(" , IL_MUL, 0, 0, 1 },
  { "DIV(" , IL_DIV, 0, 0, 1 },
  { "GT("  , IL_GT , 0, 0, 1 },
  { "GE("  , IL_GE , 0, 0, 1 },
  { "EQ("  , IL_EQ , 0, 0, 1 },
  { "NE("  , IL_NE , 0, 0, 1 },
  { "LE("  , IL_LE , 0, 0, 1 },
  { "LT("  , IL_LT , 0, 0, 1 },
  { "JMP"  , IL_JMP, 0, 0, 0 },
  { "JMPC" , IL_JMP, 1, 0, 0 },
  { "JMPCN", IL_JMP, 1, 1, 0 },
  { "JMPNC", IL_JMP, 1, 1, 0 },
  { "CAL"  , IL_CAL, 0, 0, 0 },
  { "CALC" , IL_CAL, 1, 0, 0 },
  { "CALCN", IL_CAL, 1, 1, 0 },
  { "CALNC", IL_CAL, 1, 1, 0 },
  { "RET"  , IL_RET, 0, 0, 0 },
  { "RETC" , IL_RET, 1, 0, 0 },
  { "RETCN", IL_RET, 1, 1, 0 },
  { "RETNC", IL_RET, 1, 1, 0 },
  { ")"    , IL_POP, 0, 0, 0 },
  { ""     , IL_END, 0, 0, 0 },
};

static const char *il_commands_str[] = {
        "NOP", // 0x00
        "LD",  // 0x01
        "ST",  // 0x02
        "S",   // 0x03
        "R",   // 0x04
        "AND", // 0x05
        "OR",  // 0x06
        "XOR", // 0x07
        "NOT", // 0x08
        "ADD", // 0x09
        "SUB", // 0x0a
        "MUL", // 0x0b
        "DIV", // 0x0c
        "GT",  // 0x0d
        "GE",  // 0x0e
        "EQ",  // 0x0f
        "NE",  // 0x10
        "LE",  // 0x11
        "LT",  // 0x12
        "JMP", // 0x13
        "CAL", // 0x14
        "RET", // 0x15
        "POP", // 0x16
        "???", // 0x17
        "???", // 0x18
        "???", // 0x19
        "???", // 0x1a
        "???", // 0x1b
        "???", // 0x1c
        "???", // 0x1d
        "???", // 0x1e
        "END", // 0x1f
};

static const char *lit_dataformat_str[] = {
    "LIT_BOOLEAN",       // 0x00
    "LIT_DURATION",      // 0x01
    "LIT_DATE",          // 0x02
    "LIT_TIME_OF_DAY",   // 0x03
    "LIT_DATE_AND_TIME", // 0x04
    "LIT_INTEGER",       // 0x05
    "LIT_REAL",          // 0x06
    "LIT_REAL_EXP",      // 0x07
    "LIT_BASE2",         // 0x08
    "LIT_BASE8",         // 0x09
    "LIT_BASE16",        // 0x0a
    "LIT_PHY",           // 0x0c
    "LIT_STRING",        // 0x0d
    "LIT_VAR",           // 0x0e
    "LIT_CAL",           // 0x0f
    "LIT_NONE"           // 0x10
};

static const char *pfx_dataformat[] = {
    "2#",            // 0
    "BOOL#",         // 1
    "8#",            // 2
    "16#",           // 3
    "TOD#",          // 4
    "DATE_AND_TIME#",// 5
    "DATE#",         // 6
    "TIME_OF_DAY#",  // 7
    "TIME#",         // 8
    "DT#",           // 9
    "T#",            // 10
    "D#",            // 11
    "PHY#"           // 12
};

static const uint8_t literal_format[]= {
    LIT_BASE2,         // 0
    LIT_BOOLEAN,       // 1
    LIT_BASE8,         // 2
    LIT_BASE16,        // 3
    LIT_TIME_OF_DAY,   // 4
    LIT_DATE_AND_TIME, // 5
    LIT_DATE,          // 6
    LIT_TIME_OF_DAY,   // 7
    LIT_DURATION,      // 8
    LIT_DATE_AND_TIME, // 9
    LIT_DURATION,      // 10
    LIT_DATE,          // 11
    LIT_PHY,           // 12
};

static const char *pfx_iectype[] = {
    "NULL#",    // 0
    "BOOL#",    //
    "SINT#",    //
    "USINT#",   //
    "BYTE#",    //
    "UINT#",    //
    "INT#",     //
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

static const char phy_prefix_c[] = {
    'I', //
    'Q', //
    'M', //
};

static const char phy_data_type_c[] = {
    'X', //
    'B', //
    'W', //
    'D', //
};

/////////////// load and conditioning functions ///////////////

static int find_labels(il_label_t **il_labels, String **program, int program_lines) {
    uint32_t index, pos = 0, labels_qty = 0;

    *il_labels = calloc(1, sizeof(il_label_t));

    DBG_PRINT("[LABELS]\n");
    for (uint32_t pc = 0; pc < program_lines; pc++) {
        if ((index = string_find_c((*program)[pc], ":", 0)) == STR_ERROR)
            continue;
        if ((*program)[pc]->data[index + 1] != ' ')
            continue;
        *il_labels = realloc(*il_labels, (pos + 1) * sizeof(il_label_t));

        (*il_labels)[pos].label = string_left((*program)[pc], index - 1);
        (*il_labels)[pos].line = pc;

        DBG_PRINT("    [%s line: %d]\n", (*il_labels)[pos].label->data, (*il_labels)[pos].line);

        string_delete_m((*program)[pc], 0, index);
        string_trim_m((*program)[pc]);

        ++pos;
        ++labels_qty;
    }

    if(labels_qty == 0)
        DBG_PRINT("    [NONE]\n");
    DBG_PRINT("\n");

    return labels_qty;
}

static void delete_comments(String **program, int program_lines) {
    uint32_t index1 = 0, index2 = 0;

    for (uint32_t pc = 0; pc < program_lines; pc++) {
        if ((index1 = string_find_c((*program)[pc], ";", 0)) != STR_ERROR) {
            string_delete_m((*program)[pc], index1, (*program)[pc]->length);
        }

        while((index1 = string_find_c((*program)[pc], "(*", 0)) != STR_ERROR) {
            if((index2 = string_find_c((*program)[pc], "*)", 0)) == STR_ERROR){
                printf("ERROR: unfinished comment! [%s]\n", (*program)[pc]->data);
                exit(1);
            }

            string_delete_m((*program)[pc], index1, index2 + 1);
        }

        string_trim_m((*program)[pc]);
    }
}

static void sustitute_labels(il_label_t **il_labels, int labels_qty, String **program, int program_lines) {
    uint32_t index = 0;
    char buffer[16];

    for (int pc = 0; pc < program_lines; pc++) {
        String left = string_left((*program)[pc], string_find_c((*program)[pc], " ", 0) - 1);
        string_toupper_m(left);
        if (
                !string_equals_c((*program)[pc], "JMP")   ||
                !string_equals_c((*program)[pc], "JMPC")  ||
                !string_equals_c((*program)[pc], "JMPCN") ||
                !string_equals_c((*program)[pc], "JMPNC")
        ) {
            for (uint32_t lbl = 0; lbl < labels_qty; lbl++) {
                String right = string_right((*program)[pc], string_find_c((*program)[pc], " ", 0) + 1);
                if (string_equals(right, (*il_labels)[lbl].label)) {
                    index = string_find((*program)[pc], (*il_labels)[lbl].label, 0);
                    sprintf(buffer, "%d", (*il_labels)[lbl].line);
                    string_replace_c_m((*program)[pc], (*il_labels)[lbl].label->data, buffer, index);
                }

                free(right);
            }
        }
        free(left);
    }
}

static void sustitute_others(String **program, int program_lines) {
    for (uint32_t pc = 0; pc < program_lines; pc++) {
        uint32_t spc = string_find_c((*program)[pc], " ", 0);
        String left = string_left((*program)[pc], spc - 1);
        String right = string_right((*program)[pc], spc + 1);
        // opcodes to upper
        string_toupper_m(left);

        // physical address
        string_replace_c_m(right, "%", "PHY#", 0);

        //end
        string_reset((*program)[pc]);
        String sp = string_new_c(" ");
        string_move(&(*program)[pc], &left);
        string_concat_m((*program)[pc], sp);
        string_concat_m((*program)[pc], right);
        free(sp);
        free(right);
    }
}

static int load_file(char *file, String **program) {
    FILE *f;
    uint32_t lines = 0;
    char *line_buf = NULL;
    size_t line_buf_size = 0;
    size_t line_size;
    String linebf;

    f = fopen(file, "r");
    if (f == NULL) {
        DBG_PRINT("Error: can't open file\n");
        exit(1);
    }
    printf("[FILE: %s]\n\n", file);
    *program = malloc(sizeof(String));

    while ((line_size = getline(&line_buf, &line_buf_size, f) >= 0)) {
        linebf = string_new_c(line_buf);
        if (string_isblank(linebf)) {
            free (linebf);
            continue;
        }
        string_trim_m(linebf);

        *program = realloc(*program, (lines + 1) * sizeof(String));
        (*program)[lines++] = linebf;
    }

    free(line_buf);
    fclose(f);

    return lines;
}

///////////////////////////////////////////////////////////////

/////////////////////// parse data types //////////////////////

static uint32_t identify_lit_dataformat(String value) {
    for (uint32_t n = 0; n < 13; n++) {
        if (string_find_c(value, pfx_dataformat[n], 0) != STR_ERROR) {
            return literal_format[n];
        }
    }

    if ((value->data[0] == '"' && value->data[value->length - 1] == '"') || (value->data[0] == '\'' && value->data[value->length - 1] == '\''))
        return LIT_STRING;

    if(string_isinteger(value))
        return LIT_INTEGER;

    if(string_isfloat(value))
        return LIT_REAL;

    if (string_isrealexp(value))
        return LIT_REAL_EXP;

    if ((value->data[0] == 95 || isalpha(value->data[0])) && string_isalnum(value, 0, true))
        return LIT_VAR;

    return LIT_NONE;
}

static uint32_t identify_iec_datatype(String value) {
    for (uint32_t n = 0; n < 32; n++) {
        if (string_find_c(value, pfx_iectype[n], 0) != STR_ERROR) {
            return n;
        }
    }

    return IEC_T_NULL;
}

/////////////////////// parse values //////////////////////////
static void parse_literal(String value, il_dataformat_t lit_dataformat, il_t **result);

static void parse_phy(String value, il_t **result) {
    String l,r;
    uint8_t a = 0, b = 0;
    uint16_t w;
    double d;

    (*result)->data.phy.data.bit.phy_a = 0;
    (*result)->data.phy.data.bit.phy_b = 0;
    (*result)->data.phy.data.byte = 0;
    (*result)->data.phy.data.word = 0;
    (*result)->data.phy.data.dbl = 0;

    (*result)->data.phy.prefix = PHY_P_NONE;
    (*result)->data.phy.datatype = PHY_D_BIT;

    for (uint32_t n = 0; n <= PHY_P_M; n++) {
        if (value->data[0] == phy_prefix_c[n]) {
            value->data[0] = ' ';
            (*result)->data.phy.prefix = n;
            break;
        }
    }
    if ((*result)->data.phy.prefix == PHY_P_NONE) {
        printf("ERROR: data prefix illegal! [%s]\n", value->data);
        exit(1);
    }

    for (uint32_t n = 0; n <= PHY_D_DOUBLE; n++) {
        if (value->data[1] == phy_data_type_c[n]) {
            value->data[1] = ' ';
            (*result)->data.phy.datatype = n;
            break;
        }
    }

    string_trim_m(value);

    switch ((*result)->data.phy.datatype) {
        case PHY_D_BIT:
            if ((l = string_split(value, ".", &r)) == NULL) {
                printf("ERROR: phy bit format illegal! [%s]\n", value->data);
                exit(1);
            }

            if (string_issigned(l) || string_issigned(r)) {
                printf("ERROR: phy bit illegal (number signed)! [%s]\n", value->data);
                exit(1);
            }

            if ((a = string_tolong(l, 10)) != string_tolong(l, 10) || (b = string_tolong(r, 10)) != string_tolong(r, 10)) {
                printf("ERROR: phy bit illegal (number not byte)(a: %d[%s], b: %d[%s])! [%s]\n", a, l->data, b, r->data, value->data);
                exit(1);
            }

            (*result)->data.phy.data.bit.phy_a = a;
            (*result)->data.phy.data.bit.phy_b = b;

            free(l);
            free(r);

            break;
        case PHY_D_BYTE:
            if (string_issigned(value)) {
                printf("ERROR: phy byte illegal (number signed)! [%s]\n", value->data);
                exit(1);
            }

            if ((a = string_tolong(value, 10)) != string_tolong(value, 10)) {
                printf("ERROR: phy byte illegal (number not byte)! [%s]\n", value->data);
                exit(1);
            }

            (*result)->data.phy.data.byte = a;

            break;
        case PHY_D_WORD:
            if (string_issigned(value)) {
                printf("ERROR: phy word illegal (number signed)! [%s]\n", value->data);
                exit(1);
            }

            if ((w = string_tolong(value, 10)) != string_tolong(value, 10)) {
                printf("ERROR: phy word illegal (number not word)! [%s]\n", value->data);
                exit(1);
            }

            (*result)->data.phy.data.byte = w;

            break;
        case PHY_D_DOUBLE:
            if ((d = string_todouble(value)) != string_todouble(value)) {
                printf("ERROR: phy float illegal (number not float)! [%s]\n", value->data);
                exit(1);
            }

            (*result)->data.phy.data.dbl = d;

            break;
    }
}

static void parse_string(String value, il_t **result) {
    value->data[0] = ' ';
    value->data[value->length - 1] = ' ';
    string_trim_m(value);
    (*result)->data.str = string_new_c(value->data);
}

static void parse_boolean(String value, il_t **result) {
    if (string_equals_c(value, "0") || string_equals_c(value, "FALSE"))
        (*result)->data.boolean = 0;
    else if (string_equals_c(value, "1") || string_equals_c(value, "TRUE"))
        (*result)->data.boolean = 1;
    else {
        printf("ERROR: boolean illegal! [%s]\n", value->data);
        exit(1);
    }
}

static void parse_duration(String value, il_t **result) {
    uint32_t pos = 0;

    const char *pf[4] = { "H", "M", "S", "L" };
    uint8_t *vl[4] = {
            &((*result)->data.tod.hour),
            &((*result)->data.tod.min),
            &((*result)->data.tod.sec),
            &((*result)->data.tod.msec)
    };

    string_replace_c_m(value, "MS", "L", 0);

    for (uint32_t n = 0; n < 4; n++) {
        if ((pos = string_find_c(value, pf[n], 0)) != STR_ERROR) {
            String val = string_left(value, pos - 1);
            if (!string_isinteger(val) || string_issigned(val)) {
                printf("ERROR: duration illegal! [%s]\n", val->data);
                exit(1);
            }

            if ((*vl[n] = string_tolong(val, 10)) != string_tolong(val, 10)) {
                printf("ERROR: duration illegal (number too long)! [%s]\n", val->data);
                exit(1);
            }

            string_right_m(value, pos + 1);

            free(val);
        } else
            *vl[n] = 0;
    }
}

static void parse_time_of_day(String value, il_t **result) {
    uint32_t pos;
    String v, tmp;

    if ((pos = string_find_c(value, ":", 0)) == STR_ERROR) {
        printf("ERROR: time of day illegal! [%s]\n", value->data);
        exit(1);
    }
    v = string_left(value, pos - 1);
    if (!string_isinteger(v)
            || string_issigned(v)
            || (((*result)->data.tod.hour = string_tolong(v, 10)) != string_tolong(v, 10))
            || (*result)->data.tod.hour > 23)
    {
        printf("ERROR: time of day illegal! [%s]\n", value->data);
        exit(1);
    }
    free(v);
    string_right_m(value, pos + 1);

    if ((pos = string_find_c(value, ":", 0)) == STR_ERROR) {
        printf("ERROR: time of day illegal! [%s]\n", value->data);
        exit(1);
    }
    v = string_left(value, pos - 1);
    if (!string_isinteger(v)
            || string_issigned(v)
            || (((*result)->data.tod.min = string_tolong(v, 10)) != string_tolong(v, 10))
            || (*result)->data.tod.min > 59)
    {
        printf("ERROR: time of day illegal! [%s]\n", value->data);
        exit(1);
    }
    free(v);
    string_right_m(value, pos + 1);

    if (!string_isfloat(value)
                || string_issigned(value)
                || string_todouble(value) > 59.999)
        {
        printf("ERROR: time of day illegal! [%s]\n", value->data);
        exit(1);
    } else {
        if (string_isinteger(value)) {
            (*result)->data.tod.sec = string_tolong(value, 10);
            (*result)->data.tod.msec = 0;
        } else {
            tmp = string_left(value, string_find_c(value, ".", 0) - 1);
            (*result)->data.tod.sec = string_tolong(tmp, 10);
            free(tmp);
            tmp = string_right(value, string_find_c(value, ".", 0) + 1);
            (*result)->data.tod.msec = string_tolong(tmp, 10);
            free(tmp);
        }
    }
}

static void parse_date(String value, il_t **result) {
    uint32_t pos;
    String v;

    if ((pos = string_find_c(value, "-", 0)) == STR_ERROR) {
        printf("ERROR: time of day illegal! [%s]\n", value->data);
        exit(1);
    }
    v = string_left(value, pos - 1);
    if (!string_isinteger(v)
            || string_issigned(v)
            || (((*result)->data.date.year = string_tolong(v, 10)) != string_tolong(v, 10))
            || (*result)->data.date.year < 1)
    {
        printf("ERROR: date illegal! [%s]\n", value->data);
        exit(1);
    }
    free(v);
    string_right_m(value, pos + 1);

    if ((pos = string_find_c(value, "-", 0)) == STR_ERROR) {
        printf("ERROR: time of day illegal! [%s]\n", value->data);
        exit(1);
    }
    v = string_left(value, pos - 1);
    if (!string_isinteger(v)
            || string_issigned(v)
            || (((*result)->data.date.month = string_tolong(v, 10)) != string_tolong(v, 10))
            || (*result)->data.date.month > 12 || (*result)->data.date.month < 1)
    {
        printf("ERROR: date illegal! [%s]\n", value->data);
        exit(1);
    }
    free(v);
    string_right_m(value, pos + 1);

    if (!string_isinteger(value)
            || string_issigned(value)
            || (((*result)->data.date.day = string_tolong(value, 10)) != string_tolong(value, 10))
            || (*result)->data.date.day > 31
            || (*result)->data.date.day < 1)
    {
        printf("ERROR: date illegal! [%s]\n", value->data);
        exit(1);
    }


}

static void parse_date_and_time(String value, il_t **result) {
    if (value->data[10] != '-') {
        printf("ERROR: date and time illegal! [%s]\n", value->data);
        exit(1);
    }

    il_t *val = malloc(sizeof(il_t));
    String vl;

    vl = string_left(value, 9);
    parse_date(vl, &val);
    (*result)->data.dt.date.year = val->data.date.year;
    (*result)->data.dt.date.month = val->data.date.month;
    (*result)->data.dt.date.day = val->data.date.day;
    free(vl);

    vl = string_right(value, 11);
    parse_time_of_day(vl, &val);
    (*result)->data.dt.tod.hour = val->data.tod.hour;
    (*result)->data.dt.tod.min = val->data.tod.min;
    (*result)->data.dt.tod.sec = val->data.tod.sec;
    (*result)->data.dt.tod.msec = val->data.tod.msec;
    free(vl);

    free(val);
}

static void parse_integer(String value, il_t **result) {
    if (!string_isinteger(value)) {
        printf("ERROR: integer illegal! [%s]\n", value->data);
        exit(1);
    }

    if (((*result)->data.integer = string_tolong(value, 10)) != string_tolong(value, 10)) {
        printf("ERROR: integer illegal! [%s]\n", value->data);
        exit(1);
    }
}

static void parse_real(String value, il_t **result) {
    if (!string_isfloat(value)) {
        printf("ERROR: real illegal! [%s]\n", value->data);
        exit(1);
    }

    if (((*result)->data.real = string_todouble(value)) != string_todouble(value)) {
        printf("ERROR: real illegal! [%s]\n", value->data);
        exit(1);
    }
}

static void parse_real_exp(String value, il_t **result) {
    uint8_t type = string_isrealexp(value);
    if (type == 0) {
        printf("ERROR: real exp illegal! [%s]\n", value->data);
        exit(1);
    }

    if (((*result)->data.real = string_todouble(value)) != string_todouble(value)) {
        printf("ERROR: real exp illegal! [%s]\n", value->data);
        exit(1);
    }
}

static void parse_base(String value, il_t **result) {
    string_right_m(value, string_find_c(value, "#", 0) + 1);
    printf("value: [%s](%d)\n", value->data, (int)string_tolong(value, 16));
    switch ((*result)->lit_dataformat) {
        case LIT_BASE2:
            if (((*result)->data.integer = string_tolong(value, 2)) != string_tolong(value, 2)) {
                printf("ERROR: integer illegal! [%s]\n", value->data);
                exit(1);
            }
            break;
        case LIT_BASE8:
            if (((*result)->data.integer = string_tolong(value, 8)) != string_tolong(value, 8)) {
                printf("ERROR: integer illegal! [%s]\n", value->data);
                exit(1);
            }
            break;
        case LIT_BASE16:
            string_tolower_m(value);
            if (((*result)->data.integer = string_tolong(value, 16)) != string_tolong(value, 16)) {
                printf("ERROR: integer illegal! [%s]\n", value->data);
                exit(1);
            }
            break;
        default:
            printf("ERROR: base illegal! [%s]\n", value->data);
            exit(1);
    }
}

static void parse_cal(String value, il_t **result) {
    uint32_t pos;

    pos = string_find_c(value, " ", 0);
    (*result)->data.cal.func = string_left(value, pos - 1);
    string_right_m(value, pos + 1);
    value->data[0] = ' ';
    string_delete_postfix_c_m(value, ")");
    string_trim_m(value);

    (*result)->data.cal.len = 0;
    (*result)->data.cal.value = malloc(sizeof(il_t));
    (*result)->data.cal.var = malloc(sizeof(String));

    void internal(String pos_var, il_t **result) {
        uint32_t peq;
        if ((peq = string_find_c(pos_var, ":=", 0)) == STR_ERROR) {
            printf("ERROR: cal variable illegal! [%s]\n", value->data);
            exit(1);
        }

        (*result)->data.cal.value = realloc((*result)->data.cal.value, ((*result)->data.cal.len + 1) * sizeof(il_t));
        (*result)->data.cal.var = realloc((*result)->data.cal.var, ((*result)->data.cal.len + 1) * sizeof(String));
        il_t *cv = &((*result)->data.cal.value[(*result)->data.cal.len]);

        (*result)->data.cal.var[(*result)->data.cal.len] = string_left(pos_var, peq - 1);

        String var_val = string_right(pos_var, peq + 2);
        (*result)->data.cal.value[(*result)->data.cal.len].lit_dataformat = identify_lit_dataformat(var_val);
        (*result)->data.cal.value[(*result)->data.cal.len].iec_datatype = identify_iec_datatype(var_val);

        DBG_PRINT("    [ %s (lit_dataformat: %s, iec_datatype: %s) ]\n",
                     (*result)->data.cal.var[(*result)->data.cal.len]->data,
                     lit_dataformat_str[(*result)->data.cal.value[(*result)->data.cal.len].lit_dataformat],
                     pfx_iectype[(*result)->data.cal.value[(*result)->data.cal.len].iec_datatype]
                 );

        uint32_t spc = string_find_c(var_val, "#", 0);
        string_right_m(var_val, spc + 1);

        if ((*result)->data.cal.value[(*result)->data.cal.len].lit_dataformat != LIT_STRING
                && (*result)->data.cal.value[(*result)->data.cal.len].lit_dataformat != LIT_VAR) {
            string_toupper_m(var_val);
            while (string_find_c(var_val, "_", 0) != STR_ERROR) {
                string_delete_c_m(var_val, "_");
            }
        }

        parse_literal(var_val, (*result)->data.cal.value[(*result)->data.cal.len].lit_dataformat, &cv);

        ++((*result)->data.cal.len);

        string_right_m(value, pos + 1);
        string_trim_m(value);
        free(var_val);
    }

    while ((pos = string_find_c(value, ",", 0)) != STR_ERROR) {
        String pos_var = string_left(value, pos -1);
        internal(pos_var, result);
        free(pos_var);
    }

    String pos_var = string_right(value, pos + 1);
    internal(pos_var, result);
    free(pos_var);
}

static void parse_literal(String value, il_dataformat_t lit_dataformat, il_t **result) {
    switch (lit_dataformat) {
        case LIT_BOOLEAN:
            parse_boolean(value, &((*result)));
            DBG_PRINT("        [boolean: %d]\n", (*result)->data.boolean);

            break;
        case LIT_DURATION:
            parse_duration(value, &((*result)));
            DBG_PRINT("        [H: %d, M: %d, S: %d, MS: %d]\n",
                    (*result)->data.tod.hour,
                    (*result)->data.tod.min,
                    (*result)->data.tod.sec,
                    (*result)->data.tod.msec
                    );

            break;
        case LIT_DATE:
            parse_date(value, &((*result)));
            DBG_PRINT("        [year: %d, month: %d, day: %d]\n",
                    (*result)->data.date.year,
                    (*result)->data.date.month,
                    (*result)->data.date.day
                    );
            break;
        case LIT_TIME_OF_DAY:
            parse_time_of_day(value, &((*result)));
            DBG_PRINT("        [H: %d, M: %d, S: %d, MS: %d]\n",
                    (*result)->data.tod.hour, (*result)->data.tod.min,
                    (*result)->data.tod.sec,
                    (*result)->data.tod.msec
                    );

            break;
        case LIT_DATE_AND_TIME:
            parse_date_and_time(value, &((*result)));
            DBG_PRINT("        [year: %d, month: %d, day: %d, ",
                    (*result)->data.dt.date.year, (*result)->data.dt.date.month,
                    (*result)->data.dt.date.day
                    );
            DBG_PRINT("H: %d, M: %d, S: %d, MS: %d]\n",
                    (*result)->data.dt.tod.hour,
                    (*result)->data.dt.tod.min,
                    (*result)->data.dt.tod.sec,
                    (*result)->data.dt.tod.msec
                    );
            break;
        case LIT_INTEGER:
            parse_integer(value, &((*result)));
            DBG_PRINT("        [integer: %" PRId64 "]\n", (*result)->data.integer);
            break;
        case LIT_REAL:
            parse_real(value, &((*result)));
            DBG_PRINT("        [real: %f]\n", (*result)->data.real);
            break;
        case LIT_REAL_EXP:
            parse_real_exp(value, &((*result)));
            DBG_PRINT("        [real: %f]\n", (*result)->data.real);
            break;
        case LIT_BASE2:
        case LIT_BASE8:
        case LIT_BASE16:
            parse_base(value, &((*result)));
            DBG_PRINT("        [integer: %" PRId64 "]\n", (*result)->data.integer);
            break;
        case LIT_PHY:
            parse_phy(value, &((*result)));
            DBG_PRINT("        [prefix: %d[%c], datatype: %d[%c] ",
                    (*result)->data.phy.prefix,
                    phy_prefix_c[(*result)->data.phy.prefix],
                    (*result)->data.phy.datatype,
                    phy_data_type_c[(*result)->data.phy.datatype]
                                    );
#ifdef DEBUG
            switch ((*result)->data.phy.datatype) {
                case PHY_D_BIT:
                    DBG_PRINT("phy_a: %d, phy_b: %d]\n",
                            (*result)->data.phy.data.bit.phy_a,
                            (*result)->data.phy.data.bit.phy_b
                            );
                    break;
                case PHY_D_BYTE:
                    DBG_PRINT("phy_byte: %d]\n", (*result)->data.phy.data.byte);
                    break;
                case PHY_D_WORD:
                    DBG_PRINT("phy_word: %d]\n", (*result)->data.phy.data.word);
                    break;
                case PHY_D_DOUBLE:
                    DBG_PRINT("phy_double: %f]\n", (*result)->data.phy.data.dbl);
            }
#endif
            break;
        case LIT_STRING:
            parse_string(value, &((*result)));
            DBG_PRINT("        [string: %s]\n", (*result)->data.str->data);
            break;
        case LIT_VAR:
            (*result)->data.str = string_new_c(value->data);
            DBG_PRINT("        [variable: %s]\n", (*result)->data.str->data);
            break;
        case LIT_CAL:
            parse_cal(value, &((*result)));
            break;

    }
}

///////////////////////////////////////////////////////////////

////////////////////// parse commands /////////////////////////

void parse_command(String line, il_t **result) {
    String left = NULL, right = NULL;

    string_trim_m(line);
    uint32_t spc = string_find_c(line, " ", 0);

    if (spc == STR_ERROR) {
        left = string_new_c(line->data);
        right = string_new_c("-");
    } else {
        left = string_left(line, spc - 1);
        right = string_right(line, spc + 1);
    }

    string_toupper_m(right);
    string_trim_m(left);
    string_trim_m(right);

    (*result)->code = STR_ERROR;
    (*result)->lit_dataformat = LIT_NONE;
    (*result)->iec_datatype = IEC_T_NULL;

    for (uint32_t cmd = 0; cmd < 56; cmd++) {
        if (string_equals_c(left, commands[cmd].str)) {
            (*result)->code = commands[cmd].code;
            (*result)->c = commands[cmd].c;
            (*result)->n = commands[cmd].n;
            (*result)->p = commands[cmd].p;
            break;
        }
    }
    if ((*result)->code == STR_ERROR) {
        printf("ERROR: command illegal! [%s]\n", line->data);
        exit(1);
    }
    if((*result)->code == 55)
        goto end;

    if ((*result)->code != IL_JMP && (*result)->code != IL_CAL && (*result)->code != IL_POP && (*result)->code != IL_S) {
        (*result)->lit_dataformat = identify_lit_dataformat(right);
        (*result)->iec_datatype = identify_iec_datatype(right);
    }

    end:
    free(left);
    free(right);
}

///////////////////////////////////////////////////////////////


//////////////////////// free elements ////////////////////////

static void free_labels(il_label_t **il_labels, int labels_qty) {
    for (uint32_t lbl = 0; lbl < labels_qty; lbl++) {
        free((*il_labels)[lbl].label);
    }
    free(*il_labels);
}

static void free_program(String **program, int program_lines) {
    for (uint32_t line = 0; line < program_lines; line++) {
        free((*program)[line]);
    }
    free(*program);
}

void free_il(il_t **il) {
    if (*il == NULL || il == NULL)
        return;

    switch ((*il)->lit_dataformat) {
        case LIT_STRING:
        case LIT_VAR:
            free((*il)->data.str);
            break;
        case LIT_CAL:
            for (uint32_t n = 0; n < (*il)->data.cal.len; n++) {
                free((*il)->data.cal.var[n]);
                if ((*il)->data.cal.value[n].lit_dataformat == LIT_STRING || (*il)->data.cal.value[n].lit_dataformat == LIT_VAR)
                    free((*il)->data.cal.value[n].data.str);
            }
            free((*il)->data.cal.func);
            free((*il)->data.cal.var);
            free((*il)->data.cal.value);
            break;
    }

    free(*il);
}

///////////////////////////////////////////////////////////////

void parse_file_il(char *file, parsed_il_t *parsed) {
    il_label_t *il_labels = NULL;
    uint32_t labels_qty = 0, program_lines = 0;
    String *program = NULL;
    uint32_t line;

    // program loading and conditioning
    program_lines = load_file(file, &program);
    labels_qty = find_labels(&il_labels, &program, program_lines);
    delete_comments(&program, program_lines);
    sustitute_labels(&il_labels, labels_qty, &program, program_lines);
    sustitute_others(&program, program_lines);
    free_labels(&il_labels, labels_qty);
    // ////////////////////////////////

    parsed->result = malloc(sizeof(il_t*));
    // parse program
    for (line = 0; line < program_lines; line++) {

        parsed->result = realloc(parsed->result, (line + 1) * sizeof(il_t*));
        parsed->result[line] = malloc(sizeof(il_t));

        DBG_PRINT("[%04d] %s\n", line, program[line]->data);

        parse_command(program[line], &(parsed->result[line]));
        if (parsed->result[line]->code == IL_CAL)
            parsed->result[line]->lit_dataformat = LIT_CAL;

        DBG_PRINT("    [code: %d(0x%02x)[%s], conditional: %d, negate: %d, push: %d, lit_dataformat: %d[%s], iec_datatype: %d[%s]]\n",
                parsed->result[line]->code,
                parsed->result[line]->code,
                il_commands_str[parsed->result[line]->code],
                parsed->result[line]->c,
                parsed->result[line]->n,
                parsed->result[line]->p,
                parsed->result[line]->lit_dataformat,
                lit_dataformat_str[parsed->result[line]->lit_dataformat],
                parsed->result[line]->iec_datatype,
                pfx_iectype[parsed->result[line]->iec_datatype]
                );

        String value = string_right(program[line], string_find_c(program[line], " ", 0) + 1);
        if (parsed->result[line]->code != IL_CAL) {
            uint32_t spc = string_find_c(value, "#", 0);
            string_right_m(value, spc + 1);
        }

        if (
                parsed->result[line]->lit_dataformat != LIT_STRING &&
                parsed->result[line]->lit_dataformat != LIT_VAR    &&
                parsed->result[line]->lit_dataformat != LIT_CAL
           )
        {
            string_toupper_m(value);
            while(string_find_c(value, "_", 0) != STR_ERROR) {
                string_delete_c_m(value, "_");
            }
        }

        parse_literal(value, parsed->result[line]->lit_dataformat, &(parsed->result[line]));

        free(value);
        DBG_PRINT("\n");
    }
    // /////////////

    parsed->result = realloc(parsed->result, (line + 1) * sizeof(il_t*));
    parsed->result[line] = malloc(sizeof(il_t));

    parsed->result[line]->code = IL_END;
    parsed->result[line]->iec_datatype = IEC_T_NULL;
    parsed->result[line]->lit_dataformat = LIT_NONE;
    parsed->result[line]->c = 0;
    parsed->result[line]->n = 0;
    parsed->result[line]->p = 0;
    DBG_PRINT("[%04d] END\n", line);
    DBG_PRINT("    [code: %d(0x%02x)[%s], conditional: %d, negate: %d, push: %d, lit_dataformat: %d[%s], iec_datatype: %d[%s]]\n\n",
            parsed->result[line]->code,
            parsed->result[line]->code,
            il_commands_str[parsed->result[line]->code],
            parsed->result[line]->c, parsed->result[line]->n,
            parsed->result[line]->p,
            parsed->result[line]->lit_dataformat,
            lit_dataformat_str[parsed->result[line]->lit_dataformat],
            parsed->result[line]->iec_datatype,
            pfx_iectype[parsed->result[line]->iec_datatype]
             );


    free_program(&program, program_lines);
    parsed->lines = program_lines + 1;
}
