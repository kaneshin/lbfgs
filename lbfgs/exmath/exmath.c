/*
 * File:        exmath.c
 * Version:     0.1.0
 * Maintainer:  Shintaro Kaneko <kaneshin0120@gmail.com>
 * Last Change: 01-Sep-2012.
 */

#include "include/exmath.h"

#include <stdlib.h>

double
dot_product(
    const double *x,
    const double *y,
    int n
)
{
    int i;
    double dot = x[0] * y[0];
    for (i = 1; i < n; ++i)
        dot += x[i] * y[i];
    return dot;
}

double
infinity_norm(
    const double *x,
    int n
)
{
    /*
     * norm = max(|x_0|, |x_1|, ..., |x_n|)
     */
    int i;
    double norm, fabs_x;
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

