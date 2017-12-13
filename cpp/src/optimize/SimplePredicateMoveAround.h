//
// Created by sabir on 29.08.17.
//

#ifndef OPTIMIZE_SIMPLEPREDICATEMOVEAROUND_H
#define OPTIMIZE_SIMPLEPREDICATEMOVEAROUND_H

#include "dag/DAG.h"
#include "utils/DAGVisitor.h"

// cppcheck-suppress noConstructor
class SimplePredicateMoveAround : DAGVisitor {
public:
    void optimize(DAG *dag_);

    void visit(DAGFilter *op);

private:
    std::vector<DAGFilter *> filters;
    DAG *dag;
};

#endif  // OPTIMIZE_SIMPLEPREDICATEMOVEAROUND_H
