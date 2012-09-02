/*
 * File:        backtracking_wolfe.h
 * Version:     0.1.0
 * Maintainer:  Shintaro Kaneko <kaneshin0120@gmail.com>
 * Last Change: 02-Sep-2012.
 */

#ifndef OPTIMIZATION_BACKTRACKING_WOLFE_H
#define OPTIMIZATION_BACKTRACKING_WOLFE_H

#include "../../include/nlp_component.h"

/* forward declared */
typedef struct _linesearch_parameter linesearch_parameter;

void
default_backtracking_wolfe_parameter(
    linesearch_parameter *parameter
);

int
backtracking_wolfe(
    nlp_float *work,
    const nlp_float *x,
    const nlp_float *g,
    const nlp_float *d,
    int n,
    evaluate_object *eval_obj,
    linesearch_parameter *parameter,
    nlp_component *component
);

#endif /* OPTIMIZATION_BACKTRACKING_WOLFE_H */

