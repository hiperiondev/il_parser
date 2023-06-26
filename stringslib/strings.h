/**
 * @file strings.h
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

#ifndef STRINGS_H_
#define STRINGS_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

///// core /////

/**
 * @struct string_s
 * @brief Buffered string structure
 *
 */
typedef struct string_s {
    uint32_t capacity;    /**< capacity >**/
    uint32_t length;      /**< current length >**/
        char data[];      /**< null-terminated string >**/
} string_t;               /**< Buffered string internal type >**/
typedef string_t *String; /**< Buffered string main type >**/

     String string_new(const size_t cap);
     String string_new_c(const char *str);
     String string_dup(const String buf);
   uint32_t string_move(String *to, String *from);
   uint32_t string_copy(String *to, const char *from);
       bool string_resize(String *pbuf, const size_t newcap);
       void string_reset(String buf);
const char* string_data(const String buf);

////////////////

/**
 * @enum STRING_ERROR
 * @brief Strings errors
 *
 */
enum STRING_ERROR {
    STR_OK,                 /**< Ok >**/
    STR_ERROR = UINT32_MAX, /**< Generic error >**/
};

/**
 * @enum STRING_HASH_VERSION
 * @brief String hash version
 *
 */
enum STRING_HASH_VERSION {
    SIP64,  /**< SIP64 >**/
    SIP128, /**< SIP128 >**/
    HSIP32, /**< HSIP32 >**/
    HSIP64  /**< HSIP64 >**/
};

/**
 * @struct string_hash_s
 * @brief String hash result
 *
 */
struct string_hash_s {
        uint8_t out[16]; /**< hash result >**/
         size_t outlen;  /**< hash result length >**/
};
typedef struct string_hash_s string_hash_t; /**< hash result type >**/

       String string_left(const String buf, uint32_t pos);
       String string_right(const String buf, uint32_t pos);
       String string_mid(const String buf, uint32_t left, uint32_t right);
       String string_concat(const String str1, const String str2);
       String string_insert(const String buf, const String str, uint32_t pos);
       String string_delete(const String buf, uint32_t pos1, uint32_t pos2);
       String string_delete_c(const String buf, const char *str);
       String string_delete_prefix(const String buf, const String pfx);
       String string_delete_prefix_c(const String buf, const char *pfx);
       String string_delete_postfix(const String buf, const String pfx);
       String string_delete_postfix_c(const String buf, const char *pfx);
       String string_replace(const String buf, const String search, String replace, uint32_t pos);
       String string_replace_c(const String buf, const char *c_search, const char *c_replace, uint32_t pos);
       String string_toupper(const String buf);
       String string_tolower(const String buf);
       String string_ltrim(const String buf);
       String string_rtrim(const String buf);
       String string_trim(const String buf);
       String string_split(const String buf, const char *search, String *right);
     uint32_t string_split_array(String buf, const char *search, String **array);

     uint32_t string_find(const String buf, const String search, uint32_t pos);
     uint32_t string_find_c(const String buf, const char *csearch, uint32_t pos);
     uint32_t string_append(String buf, const char *fmt, ...);
     uint32_t string_write(String buf, const char *fmt, ...);
         bool string_equals(const String str1, const String str2);
         bool string_equals_c(const String a, const char *b);
         bool string_issigned(const String buf);
         bool string_isinteger(const String buf);
         bool string_isfloat(const String buf);
         bool string_isblank(const String buf);
         bool string_isalnum(const String buf, uint32_t pos, bool underscore_dot);
      uint8_t string_isrealexp(const String buf);
         long string_tolong(const String buf, uint8_t base);
       double string_todouble(const String buf);
string_hash_t string_hash(const String buf, uint8_t version, uint8_t key[16]);

////////////////

extern String _str_result_tmp_xxxxxxx_;

/**
 * @def string_left_m
 * @brief Return to self
 *
 */
#define string_left_m(buf, pos)                                                                 \
            _str_result_tmp_xxxxxxx_ = string_left((buf), (pos));                               \
            string_move(&(buf), &_str_result_tmp_xxxxxxx_)

/**
 * @def string_right_m
 * @brief Return to self
 *
 */
#define string_right_m(buf, pos)                                                                \
            _str_result_tmp_xxxxxxx_ = string_right((buf), (pos));                              \
            string_move(&(buf), &_str_result_tmp_xxxxxxx_)

/**
 * @def string_mid_m
 * @brief Return to self
 *
 */
#define string_mid_m(buf,left,right)                                                            \
            _str_result_tmp_xxxxxxx_ = string_mid((buf), (left), (right));                      \
            string_move(&(buf), &_str_result_tmp_xxxxxxx_)

/**
 * @def string_concat_m
 * @brief Return to self
 *
 */
#define string_concat_m(buf,str2)                                                               \
            _str_result_tmp_xxxxxxx_ = string_concat((buf), (str2));                            \
            string_move(&(buf), &_str_result_tmp_xxxxxxx_)

/**
 * @def string_insert_m
 * @brief Return to self
 *
 */
#define string_insert_m(buf,str,pos)                                                            \
            _str_result_tmp_xxxxxxx_ = string_insert((buf), (str), (pos));                      \
            string_move(&(buf), &_str_result_tmp_xxxxxxx_)

/**
 * @def string_delete_m
 * @brief Return to self
 *
 */
#define string_delete_m(buf,pos1,pos2)                                                          \
            _str_result_tmp_xxxxxxx_ = string_delete((buf), (pos1), (pos2));                    \
            string_move(&(buf), &_str_result_tmp_xxxxxxx_)

/**
 * @def string_delete_c_m
 * @brief Return to self
 *
 */
#define string_delete_c_m(buf,str)                                                              \
            _str_result_tmp_xxxxxxx_ = string_delete_c((buf), (str));                           \
            string_move(&(buf), &_str_result_tmp_xxxxxxx_)

/**
 * @def string_delete_prefix_m
 * @brief Return to self
 *
 */
#define string_delete_prefix_m(buf,str)                                                         \
            _str_result_tmp_xxxxxxx_ = string_delete_prefix((buf), (str));                      \
            string_move(&(buf), &_str_result_tmp_xxxxxxx_)

/**
 * @def string_delete_prefix_c_m
 * @brief Return to self
 *
 */
#define string_delete_prefix_c_m(buf,str)                                                       \
            _str_result_tmp_xxxxxxx_ = string_delete_prefix_c((buf), (str));                    \
            string_move(&(buf), &_str_result_tmp_xxxxxxx_)


/**
 * @def string_delete_postfix_m
 * @brief Return to self
 *
 */
#define string_delete_postfix_m(buf,str)                                                        \
            _str_result_tmp_xxxxxxx_ = string_delete_postfix((buf), (str));                     \
            string_move(&(buf), &_str_result_tmp_xxxxxxx_)

/**
 * @def string_delete_postfix_c_m
 * @brief Return to self
 *
 */
#define string_delete_postfix_c_m(buf,str)                                                      \
            _str_result_tmp_xxxxxxx_ = string_delete_postfix_c((buf), (str));                   \
            string_move(&(buf), &_str_result_tmp_xxxxxxx_)

/**
 * @def string_replace_m
 * @brief Return to self
 *
 */
#define string_replace_m(buf,search,replace,pos)                                                \
            _str_result_tmp_xxxxxxx_ = string_replace((buf), (search), (replace), (pos));       \
            string_move(&(buf), &_str_result_tmp_xxxxxxx_)

/**
 * @def string_replace_c_m
 * @brief Return to self
 *
 */
#define string_replace_c_m(buf,c_search,c_replace,pos)                                          \
            _str_result_tmp_xxxxxxx_ = string_replace_c((buf), (c_search), (c_replace), (pos)); \
            string_move(&(buf), &_str_result_tmp_xxxxxxx_)

/**
 * @def string_toupper_m
 * @brief Return to self
 *
 */
#define string_toupper_m(buf)                                                                   \
            _str_result_tmp_xxxxxxx_ = string_toupper((buf));                                   \
            string_move(&(buf), &_str_result_tmp_xxxxxxx_)

/**
 * @def string_tolower_m
 * @brief Return to self
 *
 */
#define string_tolower_m(buf)                                                                   \
            _str_result_tmp_xxxxxxx_ = string_tolower((buf));                                   \
            string_move(&(buf), &_str_result_tmp_xxxxxxx_)

/**
 * @def string_ltrim_m
 * @brief Return to self
 *
 */
#define string_ltrim_m(buf)                                                                     \
            _str_result_tmp_xxxxxxx_ = string_ltrim((buf));                                     \
            string_move(&(buf), &_str_result_tmp_xxxxxxx_)

/**
 * @def string_rtrim_m
 * @brief Return to self
 *
 */
#define string_rtrim_m(buf)                                                                     \
            _str_result_tmp_xxxxxxx_ = string_rtrim((buf));                                     \
            string_move(&(buf), &_str_result_tmp_xxxxxxx_)

/**
 * @def string_trim_m
 * @brief Return to self
 *
 */
#define string_trim_m(buf)                                                                      \
            _str_result_tmp_xxxxxxx_ = string_trim((buf));                                      \
            string_move(&(buf), &_str_result_tmp_xxxxxxx_)

/**
 * @def string_splitr_m
 * @brief Return right to self
 *
 */
#define string_splitr_m(buf, search, left)                                                      \
		    (left) = string_split((buf), (search), (&_str_result_tmp_xxxxxxx_));                \
		    string_move(&(buf), &_str_result_tmp_xxxxxxx_)

/**
 * @def string_splitl_m
 * @brief Return left to self
 *
 */
#define string_splitl_m(buf, search, right)                                                     \
		    _str_result_tmp_xxxxxxx_ = string_split((buf), (search), (&right));                 \
            string_move(&(buf), &_str_result_tmp_xxxxxxx_)

#endif /* STRINGS_H_ */
