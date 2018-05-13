//
// Created by sabir on 04.07.17.
//

#ifndef DAG_DAGFILTER_H
#define DAG_DAGFILTER_H

#include "DAGOperator.h"

class DAGFilter : public DAGOperatorBase<DAGFilter> {
public:
    constexpr static const char *kName = "filter";

    DAGFilter *copy();
};

#endif  // DAG_DAGFILTER_H