/**
 * @file strings.h
 * @brief
 * @copyright 2022 Emiliano Augusto Gonzalez (hiperiondev). This project is released under MIT license. Contact: egonzalez.hiperion@gmail.com
 * @see Project Site: https://github.com/hiperiondev/il_parser
 * @note This is based on https://github.com/MattMoony/propaganda.c. Please contact their authors for more information.
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

#ifndef STRINGS_H
#define STRINGS_H

#include <stdbool.h>

typedef enum {
    STR2INT_SUCCESS,      //
    STR2INT_OVERFLOW,     //
    STR2INT_UNDERFLOW,    //
    STR2INT_INCONVERTIBLE //
} str2int_errno;

 char* strcpy_alt(char*);
 char* newstr(char*);
   int strequals(char*, char*);

 char* substr(char*, int);
 char* substring(char*, int, int);

   int indexOf(char*, char);
   int indexOfFrom(char*, char, int);
   int lastIndexOf(char*, char);

   int indexOfString(char*, char*);
   int indexOfStringFrom(char*, char*, int);
   int lastIndexOfString(char*, char*);

   int charOccurrence(char*, char);
   int stringOccurrence(char*, char*);

char** split(char*, char);
   int split2(char *str, char delimiter, char **left, char **right);
char** splits(char*, char*);

 char* lowerCase(char*);
 char* upperCase(char*);
  void toLowerCase(char*);
  void toUpperCase(char*);

  void replace(char*, char, char);
 char* replaceString(char*, char*, char*);
   int replacestr(char *line, const char *search, const char *replace);
 char* strremove(char *str, const char *sub);

 char* trim(char*);

  char itoc(int);
   int ctoi(char);

 char* itos(int);
 char* btos(int);
 char* ltos(long);
 char* ftos(float, int);
 char* dtos(double, int);

str2int_errno str2int(long int *out, char *s, int base);
          int stoi(char*);
          int stob(char*);
         long stol(char*);
        float stof(char*);
       double stod(char*);

  bool isBlank(char *line);
  bool strisinteger(char *str);
  bool strisfloat(char *str);
  bool strisrealexp(char *str);

#endif /* STRINGS_H */
