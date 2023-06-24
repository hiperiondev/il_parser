/**
 * @file strings.c
 * @brief strings manipulation functions
 * @copyright 2023 Emiliano Augusto Gonzalez (hiperiondev). This project is released under MIT license. Contact: egonzalez.hiperion@gmail.com
 * @see Project Site: https://github.com/hiperiondev/stringslib
 * @note This is based on https://github.com/alcover/buf and others. Please contact their authors for more information.
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

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <float.h>
#include <limits.h>

#include "strings.h"
#include "siphash.h"
#include "halfsiphash.h"

///// core /////

/**
 * @def BUF_CHR
 * @brief size of buffered string structure
 *
 */
#define BUF_CHR       (sizeof ((string_t){0}).data[0])

/**
 * @def BUF_MEM
 * @brief size of buffered string
 *
 */
#define BUF_MEM(cap)  (sizeof(string_t) + (cap + 1) * BUF_CHR)

/**
 * @fn String string_buf_new(const size_t cap)
 * @brief Allocate a new Buffer of capacity `cap`.
 *
 * @param cap Capacity
 * @return  Buffered string
 */
String string_new(const size_t cap) {
    String buf = calloc(1, BUF_MEM(cap));

    if (buf) {
        buf->capacity = cap;
        buf->length = 0;
        buf->data[0] = 0;
        buf->data[cap] = 0;
    }

    return buf;
}

/**
 * @fn String string_buf_new_c(const char *str)
 * @brief Allocate a new Buffer of capacity `cap` and copy string
 *
 * @param str String
 * @return  Buffered string|NULL
 */
String string_new_c(const char *str) {
    if (str == NULL || strlen(str) > UINT32_MAX - 1)
        return NULL;

    String buf = string_new(strlen(str));
    memcpy(buf->data, str, strlen(str));
    buf->length = strlen(str);

    return buf;
}

/**
 * @fn String string_buf_dup(const String buf)
 * @brief Duplicate string
 *
 * @param buf Buffered string
 * @return Buffer
 */
String string_dup(const String buf) {
    if (buf == NULL)
        return NULL;

    String ret = string_new(buf->capacity);

    if (ret) {
        // copies only up to current length
        memcpy(ret, buf, BUF_MEM(buf->length));
    }

    return ret;
}

/**
 * @fn bool string_buf_resize(String *pbuf, const size_t newcap)
 * @brief Resize capacity
 *
 * @param pbuf Buffered string
 * @param newcap New capacity
 * @return Boolean
 */
bool string_resize(String *pbuf, const size_t newcap) {
    if (pbuf == NULL || *pbuf == NULL)
        return false;

    String buf = *pbuf;

    if (newcap == buf->capacity)
        return true;

    uint32_t buflen = buf->length;

    String tmp = realloc(buf, BUF_MEM(newcap));

    if (!tmp)
        return false;

    // truncated
    if (newcap < buflen) {
        tmp->data[newcap] = 0;
        tmp->length = newcap;
    }

    tmp->capacity = newcap;
    *pbuf = tmp;

    return true;
}

/**
 * @fn void string_move(String *to, String *from)
 * @brief Copy string and free from
 *
 * @param to Buffered string
 * @param from Buffered string
 */
uint32_t string_move(String *to, String *from) {
    if (to == NULL || from == NULL || *to == NULL || *from == NULL)
        return UINT32_MAX;

    if ((*from)->length > (*to)->length)
        if (!string_resize(to, (*from)->capacity))
            return UINT32_MAX;

    memcpy((*to), (*from), BUF_MEM((*from)->length));
    (*to)->length = (*from)->length;
    free(*from);

    return 0;
}

/**
 * @fn uint32_t string_copy(String *to, const char *from)
 * @brief Copy string
 *
 * @param to Buffered string
 * @param from string
 */
uint32_t string_copy(String *to, const char *from) {
    if (to == NULL || *to == NULL || from == NULL)
        return UINT32_MAX;

    size_t lenf = strlen(from);
    if (lenf > UINT32_MAX - 1)
        return UINT32_MAX;

    if (lenf > (*to)->length)
        if (!string_resize(to, BUF_MEM(lenf)))
            return UINT32_MAX;

    memcpy((*to)->data, from, lenf + 1);
    (*to)->length = lenf;

    return 0;
}

/**
 * @fn const char* string_buf_data(const String buf)
 * @brief Return Data of Buffered string
 *
 * @param buf Buffered string
 * @return String
 */
const char* string_data(const String buf) {
    if (buf == NULL)
        return NULL;

    return buf->data;
}

/**
 * @fn void string_buf_reset(String buf)
 * @brief Reset Buffered string content
 *
 * @param buf Buffered string
 */
void string_reset(String buf) {
    if (buf == NULL)
        return;

    buf->length = 0;
    buf->data[0] = 0;
}

////////////////

String _str_result_tmp_xxxxxxx_; /**< for move macros >**/

/**
 * @fn String string_left(const String buf, uint32_t pos)
 * @brief Substring left from position
 *
 * @param buf Buffered string
 * @param pos Position
 * @return Buffered string
 */
String string_left(const String buf, uint32_t pos) {
    if (buf == NULL || pos > buf->length)
        return NULL;

    String new = string_new(pos + 1);
    memcpy(new->data, buf->data, pos + 1);
    new->data[pos + 1] = '\0';
    new->length = pos + 1;

    return new;
}

/**
 * @fn String string_right(const String buf, uint32_t pos)
 * @brief Substring right from position
 *
 * @param buf Buffered string
 * @param pos Position
 * @return Buffered string
 */
String string_right(const String buf, uint32_t pos) {
    if (buf == NULL || pos > buf->length)
        return NULL;

    String new = string_new(buf->length - pos);
    memcpy(new->data, buf->data + pos, buf->length - pos + 1);
    new->length = buf->length - pos;

    return new;
}

/**
 * @fn String string_mid(const String buf, uint32_t left, uint32_t right)
 * @brief Substring left from position left to position right
 *
 * @param buf Buffered string
 * @param left Position (start in 1)
 * @param right Position
 * @return Buffered string
 */
String string_mid(const String buf, uint32_t left, uint32_t right) {
    if (buf == NULL || right > buf->length || left > buf->length || left > right)
        return NULL;

    String new = string_new(right - left);
    memcpy(new->data, buf->data + left - 1, right - left + 1);
    new->length = right - left + 1;

    return new;
}

/**
 * @fn String string_concat(const String str1, const String str2)
 * @brief Concatenation of strings
 *
 * @param str1 Buffered string
 * @param str2 Buffered string
 * @return Buffered string
 */
String string_concat(const String str1, const String str2) {
    if (str1 == NULL || str2 == NULL)
        return NULL;

    String new = string_new(str1->length + str2->length);
    memcpy(new->data, str1->data, str1->length);
    memcpy(new->data + str1->length, str2->data, str2->length + 1);
    new->length = str1->length + str2->length;

    return new;
}

/**
 * @fn String string_insert(const String buf, const String str, uint32_t pos)
 * @brief Insert string on position
 *
 * @param buf Buffered string
 * @param str Buffered string
 * @param pos Position
 * @return Buffered string
 */
String string_insert(const String buf, const String str, uint32_t pos) {
    if (buf == NULL || str == NULL || pos > buf->length)
        return NULL;

    String new = string_new(buf->length + str->length);
    memcpy(new->data, buf->data, pos);
    memcpy(new->data + pos, str->data, str->length);
    memcpy(new->data + pos + str->length, buf->data + pos, (buf->length - pos) + 1);

    new->length = buf->length + str->length;

    return new;
}

/**
 * @fn String string_delete(const String buf, uint32_t pos1, uint32_t pos2)
 * @brief Delete substring from pos1 to pos2
 *
 * @param buf Buffered string
 * @param pos1 Position
 * @param pos2 Position
 * @return Buffered string
 */
String string_delete(const String buf, uint32_t pos1, uint32_t pos2) {
    if (buf == NULL || pos1 > buf->length || pos2 > buf->length || pos1 > pos2) {
        return NULL;
    }

    String new = string_new(buf->length - pos2 + pos1);
    memcpy(new->data, buf->data, pos1);
    memcpy(new->data + pos1, buf->data + pos2 + 1, buf->length - pos2);

    new->length = buf->length - pos2 + pos1 - 1;

    return new;
}

/**
 * @fn String string_delete_prefix(const String buf, const String pfx)
 * @brief Delete prefix
 *
 * @param buf Buffered string
 * @param pfx Buffered string
 * @return Buffered string
 */
String string_delete_prefix(const String buf, const String pfx) {
    if (buf == NULL || pfx == NULL || pfx->length < 1 || pfx->length > buf->length) {
        return NULL;
    }

    uint32_t pos = 0;
    while (pos < pfx->length && buf->data[pos] == pfx->data[pos])
        ++pos;

    if (pos != pfx->length - 1)
        return NULL;

    String new = string_new(buf->length - pfx->length + 1);
    memcpy(new->data, buf->data + pos, buf->length - pfx->length + 1);
    new->length = buf->length - pfx->length + 1;

    return new;
}

/**
 * @fn String string_delete_prefix_c(const String buf, const char *pfx)
 * @brief Delete prefix const string
 *
 * @param buf Buffered string
 * @param pfx String
 * @return Buffered string
 */
String string_delete_prefix_c(const String buf, const char *pfx) {
    if (pfx == NULL) {
        return NULL;
    }

    String prefix = string_new_c(pfx);
    String res = string_delete_prefix(buf, prefix);
    free(prefix);

    return res;
}

/**
 * @fn String string_delete_postfix(const String buf, const String pfx)
 * @brief Delete postfix
 *
 * @param buf Buffered string
 * @param pfx Buffered string
 * @return Buffered string
 */
String string_delete_postfix(const String buf, const String pfx) {
    if (buf == NULL || pfx == NULL || pfx->length < 1 || pfx->length > buf->length) {
        return NULL;
    }

    uint32_t pos = 0;
    while (pos < pfx->length && buf->data[buf->length - 1 - pos] == pfx->data[pfx->length - 1 - pos])
        ++pos;

    if (pos != pfx->length)
        return NULL;

    String new = string_new(buf->length - pfx->length );
    memcpy(new->data, buf->data, buf->length - pfx->length);
    new->length = buf->length - pfx->length;

    return new;
}

/**
 * @fn String string_delete_postfix_c(const String buf, const char *pfx)
 * @brief Delete postfix const string
 *
 * @param buf Buffered string
 * @param pfx String
 * @return Buffered string
 */
String string_delete_postfix_c(const String buf, const char *pfx) {
    if (pfx == NULL) {
        return NULL;
    }

    String postfix = string_new_c(pfx);
    String res = string_delete_postfix(buf, postfix);
    free(postfix);

    return res;
}

/**
 * @fn String string_delete_c(const String buf, const char *str)
 * @brief Delete substring str
 *
 * @param buf Buffered string
 * @param str string
 * @return Buffered string
 */
String string_delete_c(const String buf, const char *str) {
    if (buf == NULL || str == NULL) {
        return NULL;
    }

    uint32_t pos1 = string_find_c(buf, str, 0);
    if (pos1 == STR_ERROR)
        return NULL;

    uint32_t pos2 = pos1 + strlen(str) - 1;

    return string_delete(buf, pos1, pos2);
}

/**
 * @fn String string_replace(const String buf, const String search, String replace, uint32_t pos)
 * @brief Replace string
 *
 * @param buf Buffered string
 * @param search Buffered string
 * @param replace Buffered string
 * @param pos Start position
 * @return Buffered string
 */
String string_replace(const String buf, const String search, String replace, uint32_t pos) {
    if (buf == NULL || search == NULL || replace == NULL || pos > buf->length)
        return NULL;

    uint32_t fpos = string_find(buf, search, pos);
    if (fpos == STR_ERROR)
        return NULL;

    String new = string_new(buf->length - search->length + replace->length);
    memcpy(new->data, buf->data, fpos);
    memcpy(new->data + fpos, replace->data, replace->length);
    memcpy(new->data + fpos + replace->length, buf->data + search->length + fpos, buf->length - fpos - search->length);

    new->length = buf->length - search->length + replace->length;

    return new;
}

/**
 * @fn String string_replace_c(const String buf, const char *search, const char *replace, uint32_t pos)
 * @brief Replace string
 *
 * @param buf Buffered string
 * @param search string
 * @param replace string
 * @param pos Start position
 * @return Buffered string
 */
String string_replace_c(const String buf, const char *c_search, const char *c_replace, uint32_t pos) {
    if (buf == NULL || c_search == NULL || c_replace == NULL || pos > buf->length)
        return NULL;

    String search = string_new_c(c_search);
    String replace = string_new_c(c_replace);

    String newstr = string_replace(buf, search, replace, pos);

    free(search);
    free(replace);

    return newstr;
}

/**
 * @fn uint32_t string_find(const String buf, const String search, uint32_t pos)
 * @brief Find substring.
 *
 * @param buf Buffered string
 * @param search Buffered string
 * @param pos Start position
 * @return Position
 */
uint32_t string_find(const String buf, const String search, uint32_t pos) {
    if (buf == NULL || search == NULL || search->length > buf->length || pos > buf->length)
        return STR_ERROR;

    char *p;
    if ((p = strstr(buf->data + pos, search->data)) != NULL)
        return (p - buf->data);

    return STR_ERROR;
}

/**
 * @fn uint32_t string_find_c(const String buf, const char *csearch, uint32_t pos)
 * @brief Find character starting at position.
 *
 * @param buf Buffered string
 * @param csearch Searched string
 * @param pos Start position
 * @return Position
 */
uint32_t string_find_c(const String buf, const char *csearch, uint32_t pos) {
    if (buf == NULL || csearch == NULL || pos > buf->length)
        return STR_ERROR;

    String search = string_new_c(csearch);
    uint32_t p = string_find(buf, search, pos);
    free(search);

    return p;
}

/**
 * @fn String string_toupper(const String buf)
 * @brief To upper string
 *
 * @param buf Buffered string
 * @return Buffered string
 */
String string_toupper(const String buf) {
    if (buf == NULL)
        return NULL;

    String new = string_new(buf->length);
    for (int i = 0; i < buf->length; i++) {
        if (buf->data[i] >= 97 && buf->data[i] <= 122)
            new->data[i] = buf->data[i] - 32;
        else
            new->data[i] = buf->data[i];
    }

    new->length = buf->length;

    return new;
}

/**
 * @fn String string_tolower(const String buf)
 * @brief To lower string
 *
 * @param buf Buffered string
 * @return Buffered string
 */
String string_tolower(const String buf) {
    if (buf == NULL)
        return NULL;

    String new = string_new(buf->length);
    for (int i = 0; i < buf->length; i++) {
        if (buf->data[i] >= 65 && buf->data[i] <= 90)
            new->data[i] = buf->data[i] + 32;
        else
            new->data[i] = buf->data[i];
    }

    new->length = buf->length;

    return new;
}

/**
 * @fn String string_ltrim(const String buf)
 * @brief Left trim string
 *
 * @param buf Buffered string
 * @return Buffered string
 */
String string_ltrim(const String buf) {
    if (buf == NULL)
        return NULL;

    uint32_t pos1 = 0;

    while (pos1 < buf->length && isspace(buf->data[pos1]))
        ++pos1;

    String new = string_new(pos1 + (buf->length - 1) + 1);
    memcpy(new->data, buf->data + pos1, (buf->length - 1) - pos1 + 1);

    new->length = (buf->length - 1) - pos1 + 1;

    return new;
}

/**
 * @fn String string_rtrim(const String buf)
 * @brief Right trim string
 *
 * @param buf Buffered string
 * @return Buffered string
 */
String string_rtrim(const String buf) {
    if (buf == NULL)
        return NULL;

    uint32_t pos2 = buf->length - 1;

    while (pos2 >= 0 && isspace(buf->data[pos2]))
        --pos2;

    String new = string_new(pos2 + 1);
    memcpy(new->data, buf->data, pos2 + 1);

    new->length = pos2 + 1;

    return new;
}

/**
 * @fn String string_trim(const String buf)
 * @brief Trim string
 *
 * @param buf Buffered string
 * @return Buffered string
 */
String string_trim(const String buf) {
    if (buf == NULL)
        return NULL;

    uint32_t pos1 = 0, pos2 = buf->length - 1;

    while (pos1 < buf->length && isspace(buf->data[pos1]))
        ++pos1;
    while (pos2 >= 0 && isspace(buf->data[pos2]))
        --pos2;

    String new = string_new(pos1 + pos2 + 1);
    memcpy(new->data, buf->data + pos1, pos2 - pos1 + 1);

    new->length = pos2 - pos1 + 1;

    return new;
}

/**
 * @fn int string_append(String buf, const char *fmt, ...)
 * @brief Append a formatted c-string to `buf`.
 *        If new data would exceed capacity, `buf` stays unmodified.
 *
 * @param buf  Buffered string
 * @param fmt Format
 * @return Change in length.
 */
uint32_t string_append(String buf, const char *fmt, ...) {
    if (buf == NULL || fmt == NULL)
        return 0;

    const size_t spc = buf->capacity - buf->length;

    if (!spc)
        return 0;

    // get potential write length
    va_list args;
    va_start(args, fmt);
    const int len = vsnprintf(NULL, 0, fmt, args); //rem: end null not counted
    va_end(args);

    if (len > spc)
        return 0;

    char *end = buf->data + buf->length;

    errno = 0;
    va_start(args, fmt);
    int written = vsnprintf(end, spc + 1, fmt, args);
    va_end(args);

    if (written < 0) {
        *end = 0; // just in case..
        return 0;
    }

    // truncated - useless?
    if (written > spc) {
        *end = 0;
        return 0;
    }

    buf->length += written;

    return written;
}

/**
 * @fn int string_write(String buf, const char *fmt, ...)
 * @brief Write a formatted c-string at beginning of `buf`.
 *        If new data would exceed capacity, `buf` stays unmodified.
 *
 * @param buf  Buffered string
 * @param fmt Format
 * @return New length or zero on failure.
 */
uint32_t string_write(String buf, const char *fmt, ...) {
    if (buf == NULL || fmt == NULL)
        return 0;

    const size_t cap = buf->capacity;

    if (!cap)
        return 0;

    // get potential write length
    va_list args;
    va_start(args, fmt);
    const int len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);

    if (len > cap)
        return 0;

    errno = 0;
    va_start(args, fmt);
    const int written = vsnprintf(buf->data, cap + 1, fmt, args);
    va_end(args);

    if (written < 0) {
        perror("buf_write");
        return 0;
    }

    buf->length = written;

    return written;
}

/**
 * @fn string_equals(const String str1, const String str2)
 * @brief Compares two strings.
 *
 * @param str1
 * @param str2
 * @return Returns true if the strings are equal, and false if not.
 */
bool string_equals(const String str1, const String str2) {
    if (str1 == NULL || str2 == NULL || str1->length != str2->length)
        return false;

    return !memcmp(str1->data, str2->data, str1->length);
}

/**
 * @fn bool string_equals_c(const String a, const char *b)
 * @brief Compare strings equality
 *
 * @param a Buffered string
 * @param b String
 * @return Boolean
 */
bool string_equals_c(const String a, const char *b) {
    if (a == NULL || b == NULL || a->length != strlen(b))
        return false;

    return !memcmp(a->data, b, a->length);
}

////////////////////////////////////////////////////////////

/**
 * @fn bool string_issigned(const String buf)
 * @brief Check if string is signed
 *
 * @param buf Buffered string
 * @return boolean
 */
bool string_issigned(const String buf) {
    if (buf == NULL)
        return false;

    if (buf->data[0] == '-')
        return true;
    else
        return false;
}

/**
 * @fn bool string_isinteger(const String buf)
 * @brief Check if string is a valid integer
 *
 * @param buf Buffered string
 * @return boolean
 */
bool string_isinteger(const String buf) {
    if (buf == NULL)
        return false;

    int n = 0;

    if (buf->data[0] == '-')
        ++n;

    for (; n < buf->length; n++) {
        if (!isdigit(buf->data[n]))
            return false;
    }

    return true;
}

/**
 * @fn bool string_isfloat(const String buf)
 * @brief Check if string is a valid float
 *
 * @param buf Buffered string
 * @return Boolean
 */
bool string_isfloat(const String buf) {
    if (buf == NULL)
        return false;

    int n = 0;
    bool dot = false;

    if (buf->data[0] == '-')
        ++n;

    for (; n < buf->length; n++) {
        if (!isdigit(buf->data[n]) && !((buf->data[n] == '.') && !dot))
            return false;

        if (buf->data[n] == '.')
            dot = true;
    }

    return true;
}

/**
 * @fn bool string_isblank(const String buf)
 * @brief Check if string is a blank line
 *
 * @param buf Buffered string
 * @return Boolean
 */
bool string_isblank(const String buf) {
    char *ch;
    bool is_blank = true;

    for (ch = buf->data; *ch != '\0'; ++ch) {
        if (!isspace(*ch)) {
            is_blank = false;
            break;
        }
    }

    return is_blank;
}

/**
 * @fn bool string_isalnum(const String buf, uint32_t pos, bool underscore)
 * @brief Check if string only contain letters and numbers
 *
 * @param buf Buffered string
 * @param pos starting position
 * @param underscore include underscore character
 * @return Boolean
 */
bool string_isalnum(const String buf, uint32_t pos, bool underscore) {
    if (pos > buf->length)
        return 0;

    unsigned char c;
    char *b = buf->data + pos;

    while ((c = *b) && (isalnum(c) || ((c == '_' && underscore))))
        ++b;

    return *b == '\0';
}



/**
 * @fn bool string_isrealexp(const String buf)
 * @brief Check if string is a valid scientific notation
 *
 * @param buf Buffered string
 * @return Boolean (0: not valid; 1: is float; 2: is integer)
 */
uint8_t string_isrealexp(const String buf) {
    if (buf == NULL)
        return 0;

    uint8_t ret;
    String right = NULL;
    String left = string_split(buf, "e", &right);
    if (left == NULL)
        left = string_split(buf, "E", &right);

    if (left == NULL || !string_isfloat(left) || !string_isinteger(right))
        ret = 0;
    else if (string_isfloat(left))
        ret = 1;
    else
        ret = 2;

    if (left != NULL)
        free(left);
    if (right != NULL)
        free(right);

    return ret;
}

/**
 * @fn long string_tolong(const String buf)
 * @brief Convert string to integer. Max value: LONG_MAX_MAX - 1.
 *
 * @param buf Buffered string
 * @return Integer result (LONG_MAX_MAX: Error in conversion)
 */
long string_tolong(const String buf, uint8_t base) {
    if (buf == NULL)
        return LONG_MAX;

    char *end;
    errno = 0;

    long result = strtol(buf->data, &end, base);
    if ((result == LONG_MIN|| result == LONG_MAX) && ERANGE == errno)
        return LONG_MAX;

    return result;
}

/**
 * @fn double string_todouble(const String buf)
 * @brief Convert string to float. Max value: DBL_MAX - 1.
 *
 * @param buf Buffered string
 * @return Double result (DBL_MAX: Error in conversion)
 */
double string_todouble(const String buf) {
    if (buf == NULL || !(string_isfloat(buf) || string_isinteger(buf) || (string_isrealexp(buf) != 0)))
        return DBL_MAX;

    char *end;
    errno = 0;

    double result = strtod(buf->data, &end);

    if ((errno == ERANGE && (result == DBL_MAX || result == -DBL_MAX)) || (errno != 0 && result == 0.0))
        return DBL_MAX;

    return result;
}

/**
 * @fn String string_split(const String buf, const char *search, const String *right)
 * @brief Split string and return left and right Strings
 *
 * @param buf Buffered string
 * @param search string to search
 * @param right Buffered string
 * @return String Left Buffered string
 */
String string_split(const String buf, const char *search, String *right) {
    if (buf == NULL || search == NULL)
        return NULL;

    uint32_t pos = string_find_c(buf, search, 0);
    if(pos == STR_ERROR)
        return NULL;

    *right = string_right(buf, pos + strlen(search));

    return string_left(buf, pos - 1);
}

/**
 * @fn uint32_t string_split_c*(const String buf, const char *search, String **array)
 * @brief Split string in an array of strings
 *
 * @param buf Buffered string
 * @param search String to search
 * @param array Array of strings
 * @return len Array string
 */
uint32_t string_split_array(const String buf, const char *search, String **array) {
    if (buf == NULL || search == NULL)
        return 0;
    uint32_t arr_len = 0;
    uint32_t pos = 0, pos_ant = 0;

    if (string_find_c(buf, search, 0) != STR_ERROR)
        (*array) = malloc(sizeof(String));
    while ((pos = string_find_c(buf, search, pos)) != STR_ERROR) {
        (*array) = realloc((*array), (arr_len + 1) * sizeof(String));
        (*array)[arr_len] = string_mid(buf, pos_ant + 1, pos);

        pos += strlen(search);
        pos_ant = pos;
        arr_len++;
    }

    if (pos_ant < buf->length) {
        (*array) = realloc((*array), (arr_len + 1) * sizeof(String));
        (*array)[arr_len] = string_right(buf, pos_ant);
        arr_len++;
    }

    return arr_len;
}

////////////////////////////////////////////////////////////

/**
 * @fn string_hash_t string_hash(const String buf, uint8_t version, uint8_t key[16])
 * @brief String hash
 *
 * @param buf Buffered string
 * @param version enum STRING_HASH_VERSION
 * @param key Key
 * @return String hash result
 */
string_hash_t string_hash(const String buf, uint8_t version, uint8_t key[16]) {
    string_hash_t result;

    if (buf == NULL) {
        result.outlen = 0;
        return result;
    }

    const size_t lengths[4] = { 8, 16, 4, 8 };
    int len = lengths[version];
    result.outlen = len;

    if (version < 2)
        siphash(buf->data, buf->length, key, result.out, len);
    else
        halfsiphash(buf->data, buf->length, key, result.out, len);

    return result;
}
