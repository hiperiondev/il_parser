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

#ifndef IL_PARSER_H_
#define IL_PARSER_H_

#include <stdint.h>
#include <stdbool.h>

#include "strings.h"

#define DEBUG

#ifdef DEBUG
    #define DBG_PRINT(fmt, args...)  \
                printf(fmt, ##args)
#else
    #define DBG_PRINT(fmt, args...)
#endif

typedef enum COMMANDS {
//   instr  //       | modifiers |  description
    IL_NOP, //  0x00 |           |  Not operation
    IL_LD,  //  0x01 |     N     |  Loads the (negated) the value of the operand into the accumulator.
    IL_ST,  //  0x02 |     N     |  Stores the (negated) content of the accumulator in the operand.
    IL_S,   //  0x03 |           |  Sets the operand (type BOOL) to TRUE if the content of the accumulator is TRUE.
    IL_R,   //  0x04 |           |  Sets the operand (type BOOL) to FALSE if the content of the accumulator is TRUE.
    IL_AND, //  0x05 |    N,(    |  Bitwise AND of the accumulator value and (negated) operand.
    IL_OR,  //  0x06 |    N,(    |  Bitwise OR of the accumulator value and (negated) operand.
    IL_XOR, //  0x07 |    N,(    |  Bitwise exclusive OR of the accumulator value and (negated) operand.
    IL_NOT, //  0x08 |           |  Bitwise negation of the accumulator value.
    IL_ADD, //  0x09 |     (     |  Addition of the accumulator value and the operand; result is written into the accumulator.
    IL_SUB, //  0x0a |     (     |  Subtraction of the operand from the accumulator value; result is written into the accumulator.
    IL_MUL, //  0x0b |     (     |  Multiplication of accumulator value and operand; result is written into the accumulator.
    IL_DIV, //  0x0c |     (     |  Division of the accumulator value by the operand; result is written into the accumulator.
    IL_GT,  //  0x0d |     (     |  Accumulator value is greater than the operand value; write result (BOOL) into the accumulator.
    IL_GE,  //  0x0e |     (     |  Accumulator value is greater than or equal to the operand value; write result (BOOL) into the accumulator.
    IL_EQ,  //  0x0f |     (     |  Accumulator value is equal to the operand value; write result (BOOL) into the accumulator.
    IL_NE,  //  0x10 |     (     |  Accumulator value is not equal to the operand value; write result (BOOL) into the accumulator.
    IL_LE,  //  0x11 |     (     |  Accumulator value is smaller than or equal to the operand value; write result (BOOL) into the accumulator.
    IL_LT,  //  0x12 |     (     |  Accumulator value is smaller than the operand value; result (BOOL) is written into the accumulator.
    IL_JMP, //  0x13 |    CN     |  Jump to the specified jump label.
    IL_CAL, //  0x14 |    CN     |  Call of a program or a function block.
    IL_RET, //  0x15 |    CN     |  Return from subroutine.
    IL_POP, //  0x16 |           |  Pop from stack.
    IL_17,  //  0x17 |           |  not defined.
    IL_18,  //  0x18 |           |  not defined.
    IL_19,  //  0x19 |           |  not defined.
    IL_1A,  //  0x1a |           |  not defined.
    IL_1B,  //  0x1b |           |  not defined.
    IL_1C,  //  0x1c |           |  not defined.
    IL_1D,  //  0x1d |           |  not defined.
    IL_1E,  //  0x1e |           |  not defined.
    IL_END  //  0x1f |           |  Program end.
} il_commands_t;

typedef enum DATAFORMAT {
    LIT_BOOLEAN,       // 0x00
    LIT_DURATION,      // 0x01
    LIT_DATE,          // 0x02
    LIT_TIME_OF_DAY,   // 0x03
    LIT_DATE_AND_TIME, // 0x04
    LIT_INTEGER,       // 0x05
    LIT_REAL,          // 0x06
    LIT_REAL_EXP,      // 0x07
    LIT_BASE2,         // 0x08
    LIT_BASE8,         // 0x09
    LIT_BASE16,        // 0x0a
    LIT_PHY,           // 0x0b
    LIT_STRING,        // 0x0c
    LIT_VAR,           // 0x0d
    LIT_CAL,           // 0x0e
    /* ... */
    LIT_NONE
} il_dataformat_t;

typedef enum DATATYPE {
    IEC_T_NULL    = 0x00, // not value
    IEC_T_BOOL    = 0x01, // bool
    IEC_T_SINT    = 0x02, // int8_t
    IEC_T_USINT   = 0x03, // uint8_t
    IEC_T_BYTE    = 0x04, // uint8_t
    IEC_T_UINT    = 0x05, // uint16_t
    IEC_T_INT     = 0x06, // int16_t
    IEC_T_WORD    = 0x07, // uint16_t
    IEC_T_DINT    = 0x08, // int32_t
    IEC_T_UDINT   = 0x09, // uint32_t
    IEC_T_DWORD   = 0x0a, // uint32_t
    IEC_T_LINT    = 0x0b, // int64_t
    IEC_T_ULINT   = 0x0c, // uint64_t
    IEC_T_LWORD   = 0x0d, // int64_t
    IEC_T_REAL    = 0x0e, // float
    IEC_T_LREAL   = 0x0f, // double
    IEC_T_TIME    = 0x10, // time_t
    IEC_T_DATE    = 0x11, // date_t
    IEC_T_TOD     = 0x12, // tod_t
    IEC_T_DT      = 0x13, // dat_t
    IEC_T_CHAR    = 0x14, // char
    IEC_T_WCHAR   = 0x15, // wchar
    IEC_T_STRING  = 0x16, // string_t
    IEC_T_WSTRING = 0x17, // string_t
    IEC_T_POINTER = 0x18, // pointer_t
    IEC_T_TABLE   = 0x19, // table_t
    IEC_T_USER    = 0x1a, // user_t
    IEC_T_R_EDGE  = 0x1b, // bool
    IEC_T_F_EDGE  = 0x1c, // bool
    IEC_T_TIMER   = 0x1d, // timer_t
    IEC_T_VAR     = 0x1e, // variable
    IEC_T_PHY     = 0x1f, // physical address
} il_datatype_t;

typedef enum PHY_PREFIX {
    PHY_P_I,   //
    PHY_P_Q,   //
    PHY_P_M,   //
    PHY_P_NONE //
} il_phy_prefix_t;

typedef enum PHY_DATATYPE {
    PHY_D_BIT,    //
    PHY_D_BYTE,   //
    PHY_D_WORD,   //
    PHY_D_DOUBLE, //
} il_phy_datatype_t;

typedef struct il il_t;
struct il {
      il_commands_t code;               // IL code
               bool c;                  // conditional
               bool n;                  // negate
               bool p;                  // push '('
      il_datatype_t iec_datatype;       //
    il_dataformat_t lit_dataformat;     //
    union {
            bool boolean;
          String str;                   //
          double real;                  //
        uint32_t jmp_addr;              //
         int64_t integer;               //
        struct {
              il_phy_prefix_t prefix;   //
            il_phy_datatype_t datatype; //
            union {
                    struct {
                     uint32_t phy_a;    //
                     uint32_t phy_b;    //
                    } bit;              //
                     uint8_t byte;      //
                    uint16_t word;      //
                      double dbl;       //
            } data;                     //
        } phy;                          //
        struct {
            uint8_t msec;               //
            uint8_t sec;                //
            uint8_t min;                //
            uint8_t hour;               //
        } tod;                          //
        struct {
             uint8_t day;               //
             uint8_t month;             //
            uint16_t year;              //
        } date;
        struct {
            struct {
                 uint8_t day;           //
                 uint8_t month;         //
                uint16_t year;          //
            } date;                     //
            struct {
                uint8_t msec;           //
                uint8_t sec;            //
                uint8_t min;            //
                uint8_t hour;           //
            } tod;                      //
        } dt;                           //
        struct {
            uint32_t len;               //
              String func;              //
              String *var;              //
                il_t *value;            //
        } cal;                          //
    } data;                             //
};

typedef struct {
         int lines;   //
        il_t **result; //
} parsed_il_t;

void parse_file_il(char *file, parsed_il_t *parsed);
void free_il(il_t **il);

#endif /* IL_PARSER_H_ */

