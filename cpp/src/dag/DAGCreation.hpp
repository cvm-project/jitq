//
// Created by sabir on 04.07.17.
//

#ifndef CPP_DAGCREATION_H
#define CPP_DAGCREATION_H




#include <iostream>
#include <unordered_map>
#include "DAGOperator.h"
#include "DAG.h"

#define FILTER "filter"
#define MAP "map"
#define FLAT_MAP "flat_map"
#define JOIN "join"
#define RANGE "range_source"
const std::string DAG_OP_RANGE = "range_source";
const std::string DAG_ACTION = "action";
const std::string DAG_DAG = "dag";
const std::string DAG_ID = "id";
const std::string DAG_OP = "op";
const std::string DAG_FUNC = "func";
const std::string DAG_PREDS = "predecessors";
#define OP "op";
#define FUNC "func";
#define PREDS "predessors";
#define FROM "from";
#define TO "to";
#define STEP "step";
#define VALUES "values";
#define OUTPUT_TYPE "output_type";
const std::string PLUGINS_DIR = "src/op_plugins";




typedef DAGOperator *(*make_dag_function)(void); // function pointer type
typedef std::unordered_map<std::string, make_dag_function> DAGOperatorsMap;

DAG *create_dag(std::string filename = "./dag.json");
DAG *parse(std::ifstream);
DAGOperator *get_operator(std::string);
void load_plugins();

#endif //CPP_DAGCREATION_H
