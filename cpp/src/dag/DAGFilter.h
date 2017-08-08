//
// Created by sabir on 04.07.17.
//

#ifndef CPP_DAGFILTER_H
#define CPP_DAGFILTER_H


#include "operators/Operator.h"
#include "operators/FilterOperator.h"
#include "DAGOperator.h"
#include <typeinfo>

class DAGFilter : public DAGOperator {

public:
    static const std::string DAG_OP_NAME;


    static DAGOperator *make_dag_operator() {
        return new DAGFilter;
    };


    void accept(DAGVisitor &v);

    DAGFilter *getThis(){return this;}
};



#endif //CPP_DAGFILTER_H