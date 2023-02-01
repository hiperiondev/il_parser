/**
 * @file il_parser.c
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
#include "strings.h"
#include "internal_parser.h"
#include "parson.h"

#define ELEMENT_END(v)         \
            (v).str = NULL;    \
            (v).c = 0;         \
            (v).n = 0;         \
            (v).p = 0;         \
            (v).code = IL_END;

typedef struct {
        char *str; //
     uint8_t code; //
        bool c;    // conditional
        bool n;    // negate
        bool p;    // push '('
} il_str_t;

typedef struct {
    char label[512]; //
    uint32_t line;   //
} label_t;

il_str_t commands[55] = {
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
};

const char *il_commands_str[] = {
        "NOP", //
        "LD",  //
        "ST",  //
        "S",   //
        "R",   //
        "AND", //
        "OR",  //
        "XOR", //
        "NOT", //
        "ADD", //
        "SUB", //
        "MUL", //
        "DIV", //
        "GT",  //
        "GE",  //
        "EQ",  //
        "NE",  //
        "LE",  //
        "LT",  //
        "JMP", //
        "CAL", //
        "RET", //
        "POP", //
        "???", //
        "???", //
        "???", //
        "???", //
        "???", //
        "???", //
        "???", //
        "???", //
        "END", //
};

const char *lit_dataformat_str[] = {
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
    "LIT_NONE"           //
};

//////////////////////////////////

int compile_il(char *file, parsed_il_t *parsed) {
    uint8_t datatype, dataformat;
    FILE *f;
    char line[512];
    char *ln = NULL, *str_tmp = NULL;
    char *ptr;
    int index, pc = 0, pos = 0, labels_qty, err;
    label_t *labels;
    char *left = NULL, *right = NULL;
    int lines = 1;
    il_t *line_parsed = parsed->result;
    line_parsed = calloc(1, sizeof(il_t));
    line_parsed[0].code = IL_END;

    labels = calloc(1, sizeof(label_t));
    labels_qty = 0;
    f = fopen(file, "r");
    if (f == NULL) {
        DBG_PRINT("Error: can't open file\n");
        exit(1);
    }

    DBG_PRINT("<START>\n");

// first pass: localize labels
    DBG_PRINT("labels:\n");
    while (fgets(line, 512, f)) {
        if (isBlank(line))
            continue;

        ptr = strchr(line, ':');
        pc++;
        if (ptr == NULL || *(ptr + 1) != ' ')
            continue;

        index = ptr - line;
        memcpy(labels[pos].label, line, index);
        labels_qty++;
        labels[pos].line = pc;
        DBG_PRINT("[%04d] %s:\n", labels[pos].line, labels[pos].label);
        ++pos;

        labels = realloc(labels, (pos + 1) * sizeof(label_t));
    }

    if (labels_qty == 0)
        DBG_PRINT("[none]\n");
    DBG_PRINT("\n");

    rewind(f);
    pc = 0;

// second pass: compile
    DBG_PRINT("program:\n");
    while (fgets(line, 512, f)) {
        if (isBlank(line))
            continue;
        pc++;

        // erase label
        ptr = strchr(line, ':');
        if (ptr != NULL && *(ptr + 1) == ' ') {
            index = ptr - line;
            memcpy(line, ptr + 1, strlen(line) - index);
        }

        // erase comment
        replacestr(line, "(*", ";");
        ptr = strchr(line, ';');
        if (ptr != NULL) {
            index = ptr - line;
            line[index] = '\0';
        }

        replace(line, 0x27, 0x22);
        ln = trim(line);
        DBG_PRINT("  [%04d] %s\n", pc, ln);

        // parse command
        split2(ln, ' ', &left, &right);
        str_tmp = trim(left);
        toUpperCase(str_tmp);
        index = 255;
        for (pos = 0; pos < 55; pos++) {
            if (!strcmp(str_tmp, commands[pos].str)) {
                index = pos;
                break;
            }
        }
        if (index == 255) {
            printf("[%s]\nerror: command not found or malformed\n", str_tmp);
            exit(1);
        }

        // labels in JMP
        if (commands[index].code == IL_JMP) {
            for (pos = 0; pos < labels_qty; pos++) {
                if (!strcmp(labels[pos].label, right)) {
                    line_parsed[lines - 1].data.jmp_addr = labels[pos].line;
                    break;
                }
            }
            if (line_parsed[lines - 1].data.jmp_addr == 0) {
                printf("\nerror: label not exist\n%s\n", ln);
                exit(1);
            }
        }

        line_parsed[lines - 1].code = commands[index].code;
        line_parsed[lines - 1].c = commands[index].c;
        line_parsed[lines - 1].n = commands[index].n;
        line_parsed[lines - 1].p = commands[index].p;
        if (line_parsed[lines - 1].code != IL_JMP && line_parsed[lines - 1].code != IL_POP && line_parsed[lines - 1].code != IL_S
                && line_parsed[lines - 1].code != IL_R) {
            line_parsed[lines - 1].str = calloc((strlen(str_tmp) + 1), sizeof(char));
            str_tmp = trim(right);
            memcpy(line_parsed[lines - 1].str, str_tmp, strlen(str_tmp));
            line_parsed[lines - 1].str[strlen(str_tmp)] = '\0';
        }

        line_parsed = realloc(line_parsed, ++lines * sizeof(il_t));
        ELEMENT_END(line_parsed[lines - 1]);
    }

    // third pass: evaluate arguments
    DBG_PRINT("\n> identify literals\n");
    for (pos = 0; pos < lines; pos++) {
        datatype = IEC_T_NULL;
        dataformat = LIT_NONE;

        if (line_parsed[pos].str == NULL || line_parsed[pos].code == IL_JMP) {
            line_parsed[pos].datatype = datatype;
            line_parsed[pos].dataformat = dataformat;
            continue;
        }

        if(line_parsed[pos].code == IL_CAL){
            line_parsed[pos].dataformat = LIT_CAL;
            continue;
        }

        char *value;
        identify_literal(&(line_parsed[pos]), &value);

        free(line_parsed[pos].str);
        line_parsed[pos].str = calloc(strlen(value) + 1, sizeof(char));
        memcpy(line_parsed[pos].str, value, strlen(value));
        free(value);
    }

    DBG_PRINT("> parse values\n");
    for (pos = 0; pos < lines; pos++) {
        err = parse_value(&(line_parsed[pos]), pos);
        if (err != 0)
            exit(1);
    }

    line_parsed[lines - 1].str = calloc(1, sizeof(char));
    parsed->result = line_parsed;
    parsed->lines = lines;
    return lines;
}

void free_il(parsed_il_t *il) {
    for (int n = 0; n < il->lines; n++) {
        if (il->result[n].dataformat == LIT_CAL) {
            for (int m = 0; m < il->result[n].data.cal.len; m++) {
                free(il->result[n].data.cal.var[m]);
                free(il->result[n].data.cal.value[m].str);
            }
            free(il->result[n].data.cal.func);
            free(il->result[n].data.cal.var);
            free(il->result[n].data.cal.value);
        } else {
            if (il->result[n].str != NULL)
                free(il->result[n].str);
        }
    }
    free(il->result);
}

void json_values(il_t line, char **dest, char *str2, int pos, JSON_Object **root) {
    JSON_Object *root_object = *root;
    char str[2048];

    switch (line.dataformat) {
        case LIT_BOOLEAN:
            sprintf(str, "%s.%s.bool", str2, "value");
            json_object_dotset_number(root_object, str, line.data.integer);
            break;

        case LIT_TIME_OF_DAY:
        case LIT_DURATION:
            sprintf(str, "%s.%s", str2, "value.msec");
            json_object_dotset_number(root_object, str, line.data.dt.tod.msec);
            sprintf(str, "%s.%s", str2, "value.sec");
            json_object_dotset_number(root_object, str, line.data.dt.tod.sec);
            sprintf(str, "%s.%s", str2, "value.min");
            json_object_dotset_number(root_object, str, line.data.dt.tod.min);
            sprintf(str, "%s.%s", str2, "value.hour");
            json_object_dotset_number(root_object, str, line.data.dt.tod.hour);
            break;

        case LIT_DATE:
            sprintf(str, "%s.%s", str2, "value.day");
            json_object_dotset_number(root_object, str, line.data.dt.date.day);
            sprintf(str, "%s.%s", str2, "value.month");
            json_object_dotset_number(root_object, str, line.data.dt.date.month);
            sprintf(str, "%s.%s", str2, "value.year");
            json_object_dotset_number(root_object, str, line.data.dt.date.year);
            break;

        case LIT_DATE_AND_TIME:
            sprintf(str, "%s.%s", str2, "value.msec");
            json_object_dotset_number(root_object, str, line.data.dt.tod.msec);
            sprintf(str, "%s.%s", str2, "value.sec");
            json_object_dotset_number(root_object, str, line.data.dt.tod.sec);
            sprintf(str, "%s.%s", str2, "value.min");
            json_object_dotset_number(root_object, str, line.data.dt.tod.min);
            sprintf(str, "%s.%s", str2, "value.hour");
            json_object_dotset_number(root_object, str, line.data.dt.tod.hour);
            sprintf(str, "%s.%s", str2, "value.day");
            json_object_dotset_number(root_object, str, line.data.dt.date.day);
            sprintf(str, "%s.%s", str2, "value.month");
            json_object_dotset_number(root_object, str, line.data.dt.date.month);
            sprintf(str, "%s.%s", str2, "value.year");
            json_object_dotset_number(root_object, str, line.data.dt.date.year);
            break;

        case LIT_REAL_EXP:
        case LIT_REAL:
            sprintf(str, "%s.%s", str2, "value.real");
            json_object_dotset_number(root_object, str, line.data.real);
            break;

        case LIT_BASE2:
        case LIT_BASE8:
        case LIT_BASE16:
        case LIT_INTEGER:
            sprintf(str, "%s.%s", str2, "value.integer");
            json_object_dotset_number(root_object, str, line.data.integer);
            break;

        case LIT_PHY:
            sprintf(str, "%s.%s", str2, "value.prefix");
            json_object_dotset_number(root_object, str, line.data.phy.prefix);
            sprintf(str, "%s.%s", str2, "value.datatype");
            json_object_dotset_number(root_object, str, line.data.phy.datatype);
            sprintf(str, "%s.%s", str2, "value.phy_a");
            json_object_dotset_number(root_object, str, line.data.phy.phy_a);
            sprintf(str, "%s.%s", str2, "value.phy_b");
            json_object_dotset_number(root_object, str, line.data.phy.phy_b);
            break;

        case LIT_STRING:
            sprintf(str, "%s.%s", str2, "value.string");
            json_object_dotset_string(root_object, str, line.str);
            break;

        case LIT_VAR:
            sprintf(str, "%s.%s", str2, "value.variable");
            json_object_dotset_string(root_object, str, line.str);
            break;

        case LIT_NONE:
            sprintf(str, "%s.%s", str2, "value.none");
            json_object_dotset_number(root_object, str, 0);
            break;

        default:
    }

    *root = root_object;
}

int parsed2json(parsed_il_t parsed, char **dest) {
    char str[1024], str2[1024];

    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    for (int pos = 0; pos < parsed.lines; pos++) {
        sprintf(str, "program.%d.%s", pos + 1, "instruction");
        json_object_dotset_string(root_object, str, il_commands_str[parsed.result[pos].code]);
        sprintf(str, "program.%d.%s", pos + 1, "code");
        json_object_dotset_number(root_object, str, parsed.result[pos].code);
        sprintf(str, "program.%d.%s", pos + 1, "conditional");
        json_object_dotset_number(root_object, str, parsed.result[pos].c);
        sprintf(str, "program.%d.%s", pos + 1, "negate");
        json_object_dotset_number(root_object, str, parsed.result[pos].n);
        sprintf(str, "program.%d.%s", pos + 1, "push");
        json_object_dotset_number(root_object, str, parsed.result[pos].p);

        sprintf(str2, pfx_iectype[parsed.result[pos].datatype]);
        strremove(str2, "#");
        sprintf(str, "program.%d.%s", pos + 1, "argument.datatype_str");
        json_object_dotset_string(root_object, str, str2);
        sprintf(str, "program.%d.%s", pos + 1, "argument.dataformat_str");
        json_object_dotset_string(root_object, str, lit_dataformat_str[parsed.result[pos].dataformat]);

        sprintf(str, "program.%d.%s", pos + 1, "argument.datatype");
        json_object_dotset_number(root_object, str, parsed.result[pos].datatype);
        sprintf(str, "program.%d.%s", pos + 1, "argument.dataformat");
        json_object_dotset_number(root_object, str, parsed.result[pos].dataformat);

        if (parsed.result[pos].dataformat != LIT_CAL) {
            sprintf(str, "program.%d.argument.str", pos + 1);
            json_object_dotset_string(root_object, str, parsed.result[pos].str);
            sprintf(str2, "program.%d.argument", pos + 1);
            json_values(parsed.result[pos], dest, str2, pos, &root_object);
        } else {
            sprintf(str, "program.%d.%s", pos + 1, "argument.function");
            json_object_dotset_string(root_object, str, parsed.result[pos].data.cal.func);
            for (int n = 0; n < parsed.result[pos].data.cal.len; n++) {
                sprintf(str, "program.%d.argument.variables.%s.str", pos + 1, parsed.result[pos].data.cal.var[n]);
                json_object_dotset_string(root_object, str, parsed.result[pos].data.cal.value[n].str);
                sprintf(str, "program.%d.%s.%s.datatype", pos + 1, "argument.variables", parsed.result[pos].data.cal.var[n]);
                json_object_dotset_number(root_object, str, parsed.result[pos].data.cal.value[n].datatype);
                sprintf(str, "program.%d.%s.%s.dataformat", pos + 1, "argument.variables", parsed.result[pos].data.cal.var[n]);
                json_object_dotset_number(root_object, str, parsed.result[pos].data.cal.value[n].dataformat);
                sprintf(str2, "program.%d.%s.%s", pos + 1, "argument.variables", parsed.result[pos].data.cal.var[n]);
                json_values(parsed.result[pos].data.cal.value[n], dest, str2, pos, &root_object);
            }
        }
    }

    *dest = json_serialize_to_string_pretty(root_value);
    json_value_free(root_value);
    return 0;
}
