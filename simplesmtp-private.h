/**
 * @file simplesmtp-private.h
 * @brief Declarations for SimpleSmtp class
 * @author Nicu Tofan <nicu.tofan@gmail.com>
 * @copyright Copyright 2014 piles contributors. All rights reserved.
 * This file is released under the
 * [MIT License](http://opensource.org/licenses/mit-license.html)
 */

#ifndef GUARD_SIMPLESMTP_PRIVATE_H_INCLUDE
#define GUARD_SIMPLESMTP_PRIVATE_H_INCLUDE

#include <simplesmtp/simplesmtp-config.h>

#if 0
#    define SIMPLESMTP_DEBUGM printf
#else
#    define SIMPLESMTP_DEBUGM black_hole
#endif

#if 0
#    define SIMPLESMTP_TRACE_ENTRY printf("SIMPLESMTP ENTRY %s in %s[%d]\n", __func__, __FILE__, __LINE__)
#else
#    define SIMPLESMTP_TRACE_ENTRY
#endif

#if 0
#    define SIMPLESMTP_TRACE_EXIT printf("SIMPLESMTP EXIT %s in %s[%d]\n", __func__, __FILE__, __LINE__)
#else
#    define SIMPLESMTP_TRACE_EXIT
#endif


static inline void black_hole (...)
{}

#endif // GUARD_SIMPLESMTP_PRIVATE_H_INCLUDE
