/**
 * @file il.c
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

#include "IL.h"
#include "strings.h"
#include "parser.h"

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

//////////////////////////////////

int compile_il(char *file, il_t **parsed) {
    uint8_t datatype, dataformat;
    FILE *f;
    char line[512];
    char *ln = NULL, *str_tmp = NULL;
    char *ptr;
    int index, pc = 0, pos = 0, labels_qty, err;
    label_t *labels;
    char *left = NULL, *right = NULL;
    int lines = 1;
    il_t *line_parsed = *parsed;
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
        ptr = strchr(line, ';');
        if (ptr != NULL) {
            index = ptr - line;
            line[index] = '\0';
        }

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
            line_parsed[pos].data_type = datatype;
            line_parsed[pos].data_format = dataformat;
            continue;
        }

        if(line_parsed[pos].code == IL_CAL){
            line_parsed[pos].data_format = LIT_CAL;
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

    *parsed = line_parsed;
    return lines;
}

void free_il(il_t **il, int lines) {
    for (int n = 0; n < lines; n++) {
        if ((*il)[n].data_format == LIT_CAL) {
            for (int m = 0; m < (*il)[n].data.cal.len; m++) {
                free((*il)[n].data.cal.var[m]);
                free((*il)[n].data.cal.value[m].str);
            }
            free((*il)[n].data.cal.func);
            free((*il)[n].data.cal.var);
            free((*il)[n].data.cal.value);
        } else {
            if ((*il)[n].str != NULL)
                free((*il)[n].str);
        }
    }

    free(*il);
    *il = NULL;
}
