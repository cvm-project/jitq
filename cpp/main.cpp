#include <utility>
#include <cstddef>
#include <tuple>
#include <iostream>
#include "code_gen/generate_code.h"

#include "src/dag/DAGCreation.hpp"

//
//struct small_tuple{
//    long a;
//    long b;
//};
//
//struct big_tuple {
//    long a;
//    long b;
//    long c;
//    long d;
//};
//
//small_tuple func(small_tuple t){
//    return {t.a, t.b};
//}
//
int main(int argc, char **argv) {
    //get the dag string as an argument
    DAG *dag = parse_dag("[]");
    generate_code(dag);
    //compile the code to some object file

    //return

    //in python call the execute() function

    //which should return a pointer to the result
    //which should be interpreted as specified by the user
    //e.g. list or an integer(if the action is count)
    return 0;
}
