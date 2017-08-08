//
// Created by sabir on 04.07.17.
//

#ifndef CPP_DAGREDUCE_H
#define CPP_DAGREDUCE_H


#include "operators/Operator.h"
#include "operators/ReduceOperator.h"
#include "DAGOperator.h"

class DAGReduce : public DAGOperator {

public:
    static const std::string DAG_OP_NAME;


    static DAGOperator *make_dag_operator() {
        return new DAGReduce;
    };


    void accept(DAGVisitor &v);

};


#endif //CPP_DAGREDUCE_H
