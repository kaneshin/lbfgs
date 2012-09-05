/*
 * File:        lbfgs.h
 * Version:     0.1.0
 * Maintainer:  Shintaro Kaneko <kaneshin0120@gmail.com>
 * Last Change: 05-Sep-2012.
 */

#ifndef OPTIMIZATION_LBFGS_H
#define OPTIMIZATION_LBFGS_H

#include "nlp_component.h"
#include "../linesearch/include/linesearch.h"

enum lbfgs_status
{
    LBFGS_VALUE_NAN = -8,
    LBFGS_VALUE_INF,
    LBFGS_NO_FUNCTION,
    LBFGS_SATISFIED = 0,
    LBFGS_CONVERGENCE,
    LBFGS_NO_CONVERGENCE,
    LBFGS_LINESEARCH_NO_PARAMETER,
    LBFGS_LINESEARCH_FAILED,
    LBFGS_DIRECTION_SEARCH_FAILED,
    LBFGS_OUT_OF_MEMORY,
};

typedef struct _lbfgs_parameter {
    nlp_float tolerance;
    int upper_iter;
} lbfgs_parameter;


int
lbfgs(
    nlp_float *x,
    int n,
    function_object *func_obj,
    linesearch_parameter *ls_parameter,
    lbfgs_parameter *parameter
);

#endif /* OPTIMIZATION_LBFGS_H */
