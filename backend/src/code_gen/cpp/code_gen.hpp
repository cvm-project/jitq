#ifndef CODE_GEN_CPP_CODE_GEN_HPP
#define CODE_GEN_CPP_CODE_GEN_HPP

#include <string>

#include "context.hpp"
#include "dag/DAG.h"
#include "dag/type/tuple.hpp"

namespace code_gen {
namespace cpp {

std::string ComputeStructToValue(const std::string &input_var_name,
                                 const std::string &output_var_name);

std::string ComputeValueToStruct(const std::string &input_var_name,
                                 const dag::type::Tuple *tuple_type,
                                 Context *context);

FunctionDef GenerateExecuteTuples(DAG *dag, Context *context);

std::string GenerateExecuteValues(DAG *dag, Context *context);

}  // namespace cpp
}  // namespace code_gen

#endif  // CODE_GEN_CPP_CODE_GEN_HPP
