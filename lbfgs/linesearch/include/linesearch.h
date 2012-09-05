/*
 * File:        linesearch.h
 * Version:     0.1.0
 * Maintainer:  Shintaro Kaneko <kaneshin0120@gmail.com>
 * Last Change: 05-Sep-2012.
 */

#ifndef OPTIMIZATION_LINESEARCH_H
#define OPTIMIZATION_LINESEARCH_H

#include "../../include/nlp_component.h"

enum linesearch_status
{
    LINESEARCH_VALUE_NAN = -8,
    LINESEARCH_SATISFIED = 0,
    LINESEARCH_FAILED,
    LINESEARCH_STEPWIDTH_FAILED,
};

typedef struct _linesearch_parameter {
    int maxit;
    nlp_float minw;
    nlp_float maxw;
    nlp_float width;
    nlp_float beta;
    nlp_float xi;
    nlp_float sigma;
    nlp_float dec;
    nlp_float inc;
} linesearch_parameter;

void
default_linesearch_parameter(
    linesearch_parameter *parameter
);

int
linesearch(
    nlp_float *work,
    const nlp_float *x,
    const nlp_float *g,
    const nlp_float *d,
    int n,
    evaluate_object *eval_obj,
    linesearch_parameter *parameter,
    nlp_component *component
);


#endif /* OPTIMIZATION_LINE_SEARCH_COMPONENT_H */

