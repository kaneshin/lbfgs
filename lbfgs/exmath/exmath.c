/*
 * File:        exmath.c
 * Version:     0.1.0
 * Maintainer:  Shintaro Kaneko <kaneshin0120@gmail.com>
 * Last Change: 05-Sep-2012.
 */

#include "include/exmath.h"

#include <stdlib.h>

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
    for (i = 1, norm = FABS(x[0]); i < n; ++i)
    {
        fabs_x = FABS(x[i]);
        if (fabs_x > norm)
            norm = fabs_x;
    }
    return norm;
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

