/**
 * \file
 * <!--
 * This file is part of BeRTOS.
 *
 * Bertos is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As a special exception, you may use this file as part of a free software
 * library without restriction.  Specifically, if other files instantiate
 * templates or use macros or inline functions from this file, or you compile
 * this file and link it with other files to produce an executable, this
 * file does not by itself cause the resulting executable to be covered by
 * the GNU General Public License.  This exception does not however
 * invalidate any other reasons why the executable file might be covered by
 * the GNU General Public License.
 *
 * Copyright 2003, 2004, 2005 Develer S.r.l. (http://www.develer.com/)
 * Copyright 2001, 2002, 2003 Bernie Innocenti <bernie@codewiz.org>
 *
 * -->
 *
 * \brief Additional support macros for compiler independance
 *
 * \author Bernie Innocenti <bernie@codewiz.org>
 */

#ifndef BERTOS_COMPILER_H
#define BERTOS_COMPILER_H

/** Concatenate two different preprocessor tokens (allowing macros to expand) */
#define PP_CAT(x, y)           PP_CAT__(x, y)
#define PP_CAT__(x, y)         x##y
#define PP_CAT3(x, y, z)       PP_CAT(PP_CAT(x, y), z)
#define PP_CAT4(x, y, z, w)    PP_CAT(PP_CAT3(x, y, z), w)
#define PP_CAT5(x, y, z, w, j) PP_CAT(PP_CAT4(x, y, z, w), j)

/** String-ize a token (allowing macros to expand) */
#define PP_STRINGIZE(x)   PP_STRINGIZE__(x)
#define PP_STRINGIZE__(x) #x

#define UNUSED_VAR(type, name) __attribute__((__unused__)) type name

/** Issue a compilation error if the \a condition is false */
#define STATIC_ASSERT(condition)                                               \
    UNUSED_VAR(extern char, STATIC_ASSERTION_FAILED__[(condition) ? 1 : -1])

/**
 * Issue a compilation error if \a __cond is false (this can be used inside an
 * expression).
 */
#define STATIC_ASSERT_EXPR(__cond)                                             \
    (sizeof(struct { int STATIC_ASSERTION_FAILED__ : !!(__cond); }) * 0)

/* Quasi-ANSI macros */
#ifndef offsetof
/**
 * Return the byte offset of the member \a m in struct \a s.
 *
 * \note This macro should be defined in "stddef.h" and is sometimes
 *       compiler-specific (g++ has a builtin for it).
 */
#define offsetof(s, m) (size_t) & (((s *)0)->m)
#endif
#ifndef countof
/**
 * Count the number of elements in the static array \a a.
 *
 * \note This macro is non-standard, but implements a very common idiom
 */
#if defined(__GNUC__) && !defined(__cplusplus)
/*
 * Perform a compile time type checking: countof() can only
 * work with static arrays, so throw a compile time error if a
 * pointer is passed as argument.
 *
 * NOTE: the construct __builtin_types_compatible_p() is only
 * available for C.
 */
#define countof(a)                                                             \
    (sizeof(a) / sizeof(*(a)) +                                                \
     STATIC_ASSERT_EXPR(                                                       \
         !__builtin_types_compatible_p(typeof(a), typeof(&a[0]))))
#else
#define countof(a) (sizeof(a) / sizeof(*(a)))
#endif
#endif
#ifndef alignof
/**
 * Return the alignment in memory of a generic data type.
 *
 * \note We need to worry about alignment when allocating memory that
 * will be used later by unknown objects (e.g., malloc()) or, more
 * generally, whenever creating generic container types.
 */
#define alignof(type)                                                          \
    offsetof(                                                                  \
        struct {                                                               \
            char c;                                                            \
            type member;                                                       \
        },                                                                     \
        member)
#endif

#endif /* BERTOS_COMPILER_H */
