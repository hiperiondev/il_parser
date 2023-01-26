/**
 * @file strings.c
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

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>

#include "strings.h"


/**
 * @brief   The "strcpy" function returns a copy of the
 *          specified "string".
 * @param   (char*) str: some "string"
 * @return  (char*) the copy
*/

char* strcpy_alt(char *str) {
    int len = strlen(str);
    char *ret = (char*) malloc(sizeof(char) * (len + 1));

    for (int i = 0; i < len; i++)
        ret[i] = str[i];
    ret[len] = '\0';

    return ret;
}


/**
 * @brief   The "newstr" function uses the "strcpy"
 *          function to create a new "string" in the
 *          proper way.
 * @param   (char*) str: the "string"-to-be-created
 * @return  (char*) the new "string"
 */

char* newstr(char *str) {
    return strcpy_alt(str);
}

/**
 * @brief   The "strequals" function compares two
 *          "strings" and returns 1 if their equal.
 * @param   (char*) str1: the first "string"
 * @param   (char*) str2: the second "string"
 * @return  (int) 1 if equal, 0 if unequal
 */

int strequals(char *str1, char *str2) {
    int len1 = strlen(str1), len2 = strlen(str2);

    for (int i = 0; i < (len1 > len2 ? len2 : len1) + 1; i++) {
        if (str1[i] != str2[i])
            return 0;
    }
    return 1;
}

/**
 * @brief   The "substr" function can be used to create a
 *          substring of a "string" beginning at a specified
 *          index, and reaching until the end of the string.
 * @param   (char*) str: some "string"
 * @param   (int) start: the start index
 * @return  (char*) the created substring (if start is out of range: NULL)
 */

char* substr(char *str, int start) {
    int len = strlen(str);
    if (start >= len || start < 0)
        return NULL;

    char *res_s = (char*) malloc(sizeof(char) * (len - start + 1));

    for (int i = start; i < len; i++)
        res_s[i - start] = str[i];
    res_s[len - start] = '\0';

    return res_s;
}

/**
 * @brief   The "substring" function can be used to create a
 *          substring of a "string" with a specified start
 *          and end index.
 *          (start index included, end index excluded)
 * @param   (char*) str: some "string"
 * @param   (int) start: the start index
 * @param   (int)   end: the end index
 * @return  (char*) the created substring (if start or end are out of range: NULL)
 */

char* substring(char *str, int start, int end) {
    int len = strlen(str);
    if (start >= len || start < 0 || end > len || end <= 0 || end <= start)
        return NULL;

    char *res_s = (char*) malloc(sizeof(char) * (end - start + 1));

    for (int i = start; i < end; i++)
        res_s[i - start] = str[i];
    res_s[end - start] = '\0';

    return res_s;
}

/**
 * @brief   The "indexOf" function returns the index of the
 *          passed character in the passed "string".
 * @param   (char*) str: some "string"
 * @param   (char)    c: the wanted character
 * @return  (int) index of the wanted character (if character not included: -1)
 */

int indexOf(char *str, char c) {
    int i = 0;
    while (str[i] != c && str[i] != '\0')
        i++;

    if (str[i] != c)
        return -1;
    return i;
}

/**
 * @brief   The "indexOfFrom" function returns the first index
 *          of the specified character, beginning from i.
 * @param   (char*) str: some "string"
 * @param   (char)    c: the specified character
 * @param   (int)     i: the starting index
 * @return  (int) the first index of the wanted character (if character not included: -1)
 */

int indexOfFrom(char *str, char c, int i) {
    while (str[i] != c && str[i] != '\0')
        i++;

    if (str[i] != c)
        return -1;
    return i;
}

/**
 * @brief   The "lastIndexOf" function returns the last index
 *          of the specified character in the passed "string".
 * @param   (char*) str: some "string"
 * @param   (char)    c: the target character
 * @return  (int) last index of the target character (if character not included: -1)
 */

int lastIndexOf(char *str, char c) {
    int i = strlen(str) - 1;
    while (str[i] != c && i >= 0)
        i--;

    if (str[i] != c)
        return -1;
    return i;
}

/**
 * @brief   The "indexOfString" function returns the index of
 *          the specified substring in the "string".
 * @param   (char*) str: some "string"
 * @param   (char*) sub: the substring
 * @return  (int) index of the substring (if substring not included: -1)
 */

int indexOfString(char *str, char *sub) {
    int lenStr = strlen(str), lenSub = strlen(sub);

    if (lenSub > lenStr)
        return -1;

    int i = 0;
    while (i <= lenStr - lenSub && !strequals(sub, substring(str, i, i + lenSub))) {
        i++;
    }

    if (i > lenStr - lenSub || !strequals(sub, substring(str, i, i + lenSub)))
        return -1;
    return i;
}

/**
 * @brief   The "indexOfStringFrom" function returns the first index
 *          of the specified substring starting fromt the specified
 *          index.
 * @param   (char*) str: some "string"
 * @param   (char*) sub: some substring
 * @param   (int)     i: the starting index
 * @return  (int) the first index (if substring not included: -1)
 */

int indexOfStringFrom(char *str, char *sub, int i) {
    int lenStr = strlen(str), lenSub = strlen(sub);

    if (lenSub > lenStr - i)
        return -1;

    while (i <= lenStr - lenSub && !strequals(sub, substring(str, i, i + lenSub))) {
        i++;
    }

    if (i > lenStr - lenSub || !strequals(sub, substring(str, i, i + lenSub)))
        return -1;
    return i;
}

/**
 * @brief   The "lastIndexOfString" function returns the index
 *          of the last occurrence of the substring in the
 *          "string".
 * @param   (char*) str: some "string"
 * @param   (char*) sub: the substring
 * @return  (int) last index of the substring (if substring not included: -1)
 */

int lastIndexOfString(char *str, char *sub) {
    int lenStr = strlen(str), lenSub = strlen(sub);

    if (lenSub > lenStr)
        return -1;

    int i = lenStr - lenSub;
    while (i >= 0 && !strequals(sub, substring(str, i, i + lenSub))) {
        i--;
    }

    if (i < 0 || !strequals(sub, substring(str, i, i + lenSub)))
        return -1;
    return i;
}

/**
 * @brief   The "charOccurrence" function returns the amount
 *          of times the specified character appears in the
 *          specified "string".
 * @param   (char*) str: some "string"
 * @param   (char)    c: the specified character
 * @return  (int) the amount of times the character appears
 */

int charOccurrence(char *str, char c) {
    int amount = 0;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == c)
            amount++;
    }

    return amount;
}

/**
 * @brief   The "stringOccurrence" function returns the amount
 *          of times the specified string appears in the specified
 *          "string".
 * @param   (char*) str: the "string"
 * @param   (char*) sub: the searched "string"
 * @return  (int) the amount of times the "string" appears
 */

int stringOccurrence(char *str, char *sub) {
    int lenStr = strlen(str), lenSub = strlen(sub);

    if (lenSub > lenStr)
        return 0;

    int amount = 0;
    char *s_str;

    for (int i = 0; i < lenStr - lenSub; i++) {
        s_str = substring(str, i, i + lenSub);
        if (strequals(sub, s_str))
            amount++;
    }

    return amount;
}

/**
 * @brief   The "split" function splits a "string" at the specified
 *          character and returns the parts.
 * @param   (char*) str: some "string"
 * @param   (char)    c: the "split" character
 * @return  (char**) the individual strings
 */

char** split(char *str, char c) {
    int occ = charOccurrence(str, c);
    char *cpy = strcpy_alt(str);
    char **parts = (char**) malloc(sizeof(char*) * (occ + 1));

    for (int i = 0; i < occ; i++) {
        parts[i] = substring(cpy, 0, indexOf(cpy, c));
        cpy = substr(cpy, indexOf(cpy, c) + 1);
    }
    parts[occ] = cpy;

    return parts;
}

int split2(char *str, char delimiter, char **left, char **right) {
    int pos = 0;

    if(charOccurrence(str, delimiter) == 0) {
        *left = str;
        *right = NULL;
        return -1;
    }

    while (str[pos] != delimiter && str[pos] != '\0')
        ++pos;

    if (pos == strlen(str) + 1)
        return -2;

    *left = str;
    (*left)[pos] = '\0';
    *right = str + pos + 1;

    return 0;
}

/**
 * @brief   The "splits" function splits a "string" at the
 *          specified substring and returns the parts.
 * @param   (char*) str: some "string"
 * @param   (char*)   s: the substring
 * @return  (char**) the individual parts
 */

char** splits(char *str, char *s) {
    int occ = stringOccurrence(str, s);
    char *cpy = strcpy_alt(str);
    char **parts = (char**) malloc(sizeof(char*) * (occ + 1));

    int lenSub = strlen(s);

    for (int i = 0; i < occ; i++) {
        parts[i] = substring(cpy, 0, indexOfString(cpy, s));
        cpy = substr(cpy, indexOfString(cpy, s) + lenSub);
    }
    parts[occ] = cpy;

    return parts;
}

/**
 * @brief   The "lowerCase" function returns the lowercase
 *          representation of the specified "string".
 * @param   (char*) str: some "string
 * @return  (char*) the lowercase representation
 */

char* lowerCase(char *str) {
    int len = strlen(str);
    char *res_s = (char*) malloc(sizeof(char) * (len + 1));

    for (int i = 0; i < len; i++) {
        if (str[i] >= 65 && str[i] <= 90)
            res_s[i] = str[i] + 32;
        else
            res_s[i] = str[i];
    }

    return res_s;
}

/**
 * @brief   The "upperCase" function returns the uppercase
 *          representation of a specified "string".
 * @param   (char*) str: some "string"
 * @return  (char*) the uppercase representation
 */

char* upperCase(char *str) {
    int len = strlen(str);
    char *res_s = (char*) malloc(sizeof(char) * (len + 1));

    for (int i = 0; i < len; i++) {
        if (str[i] >= 97 && str[i] <= 122)
            res_s[i] = str[i] - 32;
        else
            res_s[i] = str[i];
    }

    return res_s;
}

/**
 * @brief   The "toLowerCase" function converts the
 *          referenced "string" to lowercase.
 * @param   (char*) str: the target "string"
 */

void toLowerCase(char *str) {
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] >= 65 && str[i] <= 90)
            str[i] = str[i] + 32;
    }
}

/**
 * @brief   The "toUpperCase" function converts the
 *          referenced "string" to uppercase.
 * @param   (char*) str: the target "string"
 */

void toUpperCase(char *str) {
    for (int i = 0; i < strlen(str); i++)
            str[i] = toupper(str[i]);
}

/**
 * @brief   The "replace" function replaces characters
 *          of one "string" with other characters.
 * @param   (char*) str: some "string"
 * @param   (char)    c: the target character
 * @param   (char)    r: the replacement character
 */

void replace(char *str, char c, char r) {
    int t_ind;
    int l_ind = 0;

    while ((t_ind = indexOfFrom(str, c, l_ind)) >= 0) {
        str[t_ind] = r;
        l_ind = t_ind + 1;
    }
}

/**
 * @brief   The "replaceString" function replaces
 *          substrings of a "string" with other
 *          substrings.
 * @param   (char*) str: some "string"
 * @param   (char*)   s: the target substring
 * @param   (char*)   r: the replacement substring
 * @return  (char*) the resulting "string"
 */

char* replaceString(char *str, char *s, char *r) {
    int t_ind, l_ind = 0;
    int lenR = strlen(r), lenS = strlen(s);
    char *after;

    while ((t_ind = indexOfStringFrom(str, s, l_ind)) >= 0) {
        after = substr(str, t_ind + lenS);
        str[t_ind] = '\0';

        str = strcat(str, r);
        str = strcat(str, after);

        l_ind = t_ind + lenR;
    }

    return str;
}

/**
 *
 */
int replacestr(char *line, const char *search, const char *replace) {
	int count;
	char *sp; // start of pattern

	if ((sp = strstr(line, search)) == NULL) {
		return (0);
	}
	count = 1;
	int sLen = strlen(search);
	int rLen = strlen(replace);
	if (sLen > rLen) {
		// move from right to left
		char *src = sp + sLen;
		char *dst = sp + rLen;
		while ((*dst = *src) != '\0') {
			dst++;
			src++;
		}
	} else if (sLen < rLen) {
		// move from left to right
		int tLen = strlen(sp) - sLen;
		char *stop = sp + rLen;
		char *src = sp + sLen + tLen;
		char *dst = sp + rLen + tLen;
		while (dst >= stop) {
			*dst = *src;
			dst--;
			src--;
		}
	}
	memcpy(sp, replace, rLen);

	count += replacestr(sp + rLen, search, replace);

	return (count);
}

/**
 * @brief   The "trim" function removes unwanted whitespace
 *          characters from before and after the "string"'s
 *          actual content.
 * @param   (char*) str: some "string"
 * @return  (char*) the trimmed string
 */

char* trim(char *str) {
    int left = 0;
    while (str[left] <= 32) {
        left++;
    }

    str = substr(str, left);

    int right = strlen(str) - 1;
    while (str[right] <= 32) {
        right--;
    }

    str = substring(str, 0, right + 1);
    return str;
}

/**
 * @brief   The "itoc" function converts an integer to a
 *          character. (e.g.: 8 -> '8')
 * @param   (int) n: the integer
 * @return  (char) the char
 */

char itoc(int n) {
    n %= 10;
    return (char) (n + 48);
}

/**
 * @brief   The "ctoi" function converts a character to
 *          an integer. (e.g.: '8' -> 8)
 * @param   (char) c: the character
 * @return  (int) the integer (if character not in range: -1)
 */

int ctoi(char c) {
    if (c < 48 || c > 57)
        return -1;
    return (int) c - 48;
}

/**
 * @brief   The "itos" function converts an integer to a
 *          "string".
 * @param   (int) n: The integer
 * @return  (char*) the "string"
 */

char* itos(int n) {
    if (n == 0)
        return newstr("0");

    int h_pot = -1;
    while ((double) n / pow(10.0, (double) (++h_pot)) >= 1.0) {
    }

    char *res_s = (char*) malloc(sizeof(char) * (h_pot + 1));
    for (int i = h_pot - 1; i >= 0; i--) {
        res_s[h_pot - i - 1] = itoc(n / (int) pow(10.0, (double) i));
        n %= (int) pow(10.0, (double) i);
    }
    res_s[h_pot] = '\0';

    return res_s;
}

/**
 * @brief   The "btos" function converts a boolean value to
 *          a string. (e.g.: 1 -> "true")
 * @param   (int) b: the boolean value
 * @return  (char*) the "string"
 */

char* btos(int b) {
    return b ? newstr("true") : newstr("false");
}

/**
 * @brief   The "ltos" function converts a long to a
 *          "string".
 * @param   (long) n: The long
 * @return  (char*) the "string"
 */

char* ltos(long n) {
    if (n == 0l)
        return newstr("0");

    int h_pot = -1;
    while ((double) n / pow(10.0, (double) (++h_pot)) >= 1.0) {
    }

    char *res_s = (char*) malloc(sizeof(char) * (h_pot + 1));
    for (int i = h_pot - 1; i >= 0; i--) {
        res_s[h_pot - i - 1] = itoc(n / (int) pow(10.0, (double) i));
        n %= (int) pow(10.0, (double) i);
    }
    res_s[h_pot] = '\0';

    return res_s;
}

/**
 * @brief   The "ftos" function converts a float to a
 *          "string".
 * @param   (float) n: The float
 * @param   (int)   l: the amount of decimals to round to
 * @return  (char*) the "string"
 */

char* ftos(float n, int l) {
    int before_c = (int) n;
    n -= (float) before_c;

    char *res_s = itos(before_c);
    res_s = strcat(res_s, newstr("."));

    for (int i = 0; i < l - 1; i++) {
        n *= 10;
        res_s = strcat(res_s, itos((int) n));
        n -= (int) n;
    }

    n *= 10;
    int diq = (int) n;
    n -= diq;

    if (n >= 0.5f)
        diq++;
    res_s = strcat(res_s, itos(diq));

    return res_s;
}

/**
 * @brief   The "dtos" function converts a double to a
 *          "string".
 * @param   (float) n: The double
 * @param   (int)   l: the amount of decimals to round to
 * @return  (char*) the "string"
 */

char* dtos(double n, int l) {
    int before_c = (int) n;
    n -= (float) before_c;

    char *res_s = itos(before_c);
    res_s = strcat(res_s, newstr("."));

    for (int i = 0; i < l - 1; i++) {
        n *= 10;
        res_s = strcat(res_s, itos((int) n));
        n -= (int) n;
    }

    n *= 10;
    int diq = (int) n;
    n -= diq;

    if (n >= 0.5)
        diq++;
    res_s = strcat(res_s, itos(diq));

    return res_s;
}

/**
 * @brief   The "stoi" function converts a string to an
 *          integer.
 * @param   (char*) str: the "string"
 * @return  (int) the integer
 */

int stoi(char *str) {
    int lenStr = strlen(str);
    int res = 0;

    for (int i = lenStr - 1; i >= 0; i--) {
        res += ctoi(str[lenStr - i - 1]) * (int) pow(10.0, i);
    }

    return res;
}

/**
 * @brief   The "stob" function converts a string to a
 *          boolean value.
 * @param   (char*) str: the "string"
 * @return  (int) the boolean value
 */

int stob(char *str) {
    return strequals(str, "true");
}

/**
 * @brief   The "stol" function converts a string to a
 *          long value.
 * @param   (char*) str: the "string"
 * @return  (long) the long
 */

long stol(char *str) {
    int lenStr = strlen(str);
    long res = 0;

    for (int i = lenStr - 1; i >= 0; i--) {
        res += ctoi(str[lenStr - i - 1]) * (long) pow(10.0, i);
    }

    return res;
}

/**
 * @brief   The "stof" function converts a string to a
 *          float.
 * @param   (char*) str: the "string"
 * @return  (float) the float
 */

float stof(char *str) {
    if (indexOf(str, '.') < 0)
        return 0.0f;

    float res = (float) stoi(substring(str, 0, indexOf(str, '.')));

    char *after_c = substr(str, indexOf(str, '.') + 1);
    for (int i = 0; i < strlen(after_c); i++) {
        res += ctoi(after_c[i]) * (float) pow(10, -i - 1);
    }

    return res;
}

/**
 * @brief   The "stod" function converts a string to a
 *          double.
 * @param   (char*) str: the "string"
 * @return  (double) the double
 */

double stod(char *str) {
    if (indexOf(str, '.') < 0)
        return 0.0;

    double res = (double) stoi(substring(str, 0, indexOf(str, '.')));

    char *after_c = substr(str, indexOf(str, '.') + 1);
    for (int i = 0; i < strlen(after_c); i++) {
        res += ctoi(after_c[i]) * pow(10, -i - 1);
    }

    return res;
}

/**
 *
 */
bool isBlank(char *line) {
    char *ch;
    bool is_blank = true;

    for (ch = line; *ch != '\0'; ++ch) {
        if (!isspace(*ch)) {
            is_blank = false;
            break;
        }
    }

    return is_blank;
}

/**
 *
 */
char* strremove(char *str, const char *sub) {
    char *p, *q, *r;
    if (*sub && (q = r = strstr(str, sub)) != NULL) {
        size_t len = strlen(sub);
        while ((r = strstr(p = r + len, sub)) != NULL) {
            while (p < r)
                *q++ = *p++;
        }
        while ((*q++ = *p++) != '\0')
            continue;
    }
    return str;
}

/**
 *
 */
bool strisinteger(char *str) {
	for (int n = 0; n < strlen(str); n++) {
		if (!isdigit(str[n]) && !(str[n] == '-'))
			return false;
	}

	return true;
}

/**
 *
 */
bool strisfloat(char *str) {
    for (int n = 0; n < strlen(str); n++) {
        if (!isdigit(str[n]) && !(str[n] == '.') && !(str[n] == '-'))
            return false;
    }

    return true;
}

/**
 *
 */
bool strisrealexp(char *str) {
    for (int n = 0; n < strlen(str); n++) {
        if (!isdigit(str[n]) && !(str[n] == '.') && !(str[n] == '-') && !((str[n] == 'e') || (str[n] == 'E')))
            return false;
    }

    return true;
}

/**
 *
 */
str2int_errno str2int(long int *out, char *s, int base) {
    if(s == NULL)
        return STR2INT_INCONVERTIBLE;

    char *end;
    if (s[0] == '\0' || isspace(s[0]))
        return STR2INT_INCONVERTIBLE;
    errno = 0;
    long l = strtol(s, &end, base);
    /* Both checks are needed because INT_MAX == LONG_MAX is possible. */
    if (l > INT_MAX || (errno == ERANGE && l == LONG_MAX))
        return STR2INT_OVERFLOW;
    if (l < INT_MIN || (errno == ERANGE && l == LONG_MIN))
        return STR2INT_UNDERFLOW;
    if (*end != '\0')
        return STR2INT_INCONVERTIBLE;
    *out = l;
    return STR2INT_SUCCESS;
}
