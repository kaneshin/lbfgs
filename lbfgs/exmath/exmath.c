/*
 * File:        exmath.c
 * Version:     1.0.0
 * Maintainer:  Shintaro Kaneko <kaneshin0120@gmail.com>
 * Last Change: 06-Sep-2012.
 *
 * License:     The MIT License (MIT)
 *
 * Copyright (c) 2012 Shintaro Kaneko <kaneshin0120@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "include/exmath.h"

nlp_float
dot_product(
    const nlp_float *x,
    const nlp_float *y,
    int n
)
{
    int i;
    nlp_float dot = x[0] * y[0];
    for (i = 1; i < n; ++i)
        dot += x[i] * y[i];
    return dot;
}

nlp_float
infinity_norm(
    const nlp_float *x,
    int n
)
{
    /*
     * norm = max(|x_0|, |x_1|, ..., |x_n|)
     */
    int i;
    nlp_float norm, fabs_x;
    for (i = 1, norm = fabs(x[0]); i < n; ++i)
    {
        fabs_x = fabs(x[i]);
        if (fabs_x > norm)
            norm = fabs_x;
    }
    return norm;
}

void*
malloc_vec(
    size_t mem_size
)
{
    void *mem = (void *)malloc(mem_size);
    return mem;
}

void
free_vec(
    void *mem
)
{
    if (NULL != mem)
    {
        free(mem);
        mem = NULL;
    }
}

void**
malloc_mat(
    size_t mem_size
)
{
    void **mem = (void **)malloc(mem_size);
    return mem;
}

void
free_mat(
    void **mem
)
{
    if (NULL != mem)
    {
        if (NULL != *mem)
        {
            free(*mem);
            *mem = NULL;
        }
        free(mem);
        mem = NULL;
    }
}

