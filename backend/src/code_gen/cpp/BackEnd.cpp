#include "BackEnd.hpp"

#include <fstream>
#include <regex>
#include <set>
#include <string>
#include <vector>

#include <boost/algorithm/string/join.hpp>
#include <boost/format.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <sys/stat.h>

#include "CodeGenVisitor.h"
#include "dag/DAGOperators.h"
#include "dag/utils/apply_visitor.hpp"
#include "utils/utils.h"

using boost::adaptors::transformed;
using boost::algorithm::join;
using boost::format;

namespace code_gen {
namespace cpp {

std::string generatePlanDriver(const CodeGenVisitor::OperatorDesc &sink) {
    return (format("/** collecting the result **/"
                   "%1%.open();"
                   "const auto result = %1%.next().value;"
                   "auto ret = (result_type*)malloc(sizeof(result_type));"
                   "ret->data = result.v0.data;"
                   "ret->size = result.v0.shape[0];"
                   "%1%.close();"
                   "return ret;") %
            sink.var_name)
            .str();
}

void BackEnd::GenerateCode(DAG *const dag) {
    const std::string genDir = get_lib_path() + "/backend/gen/";
    exec(("bash -c 'rm -r -f " + genDir + "/*'").c_str());

    // Create output directory
    const int dirErr = system(("mkdir -p " + genDir).c_str());
    if (0 != dirErr) {
        std::cerr << ("Error creating gen directory!") << std::endl;
        std::exit(1);
    }

    // Setup visitor and run it
    std::string llvmCodePath = genDir + "llvm_funcs.ll";
    std::ofstream llvmCode(llvmCodePath);

    std::stringstream planBody;
    std::stringstream planTupleDeclarations;
    std::stringstream planLLVMDeclarations;

    CodeGenVisitor visitor(dag, planBody, planTupleDeclarations, llvmCode,
                           planLLVMDeclarations);
    dag::utils::ApplyInReverseTopologicalOrder(dag, visitor.functor());

    // Compute execute function parameters
    const auto input_formatter = [](auto i) {
        return (format("void * %s, unsigned long %s") % i.first % i.second)
                .str();
    };
    const auto input_args =
            join(visitor.inputNames | transformed(input_formatter), ", ");

    // Generate typedef for result wrapper
    auto sink = visitor.operatorNameTupleTypeMap[dag->sink->id];
    auto return_type =
            visitor.operatorNameTupleTypeMap[dag->predecessor(dag->sink)->id]
                    .return_type;

    const std::string result_wrapper = (format("typedef struct {"
                                               "   unsigned long size;"
                                               "   %s *data;"
                                               "} result_type;\n") %
                                        return_type->name)
                                               .str();

    // Main executable file: declarations
    std::ofstream mainSourceFile(genDir + "execute.cpp");

    mainSourceFile << "/**\n"
                      " * Auto-generated execution plan\n"
                      " */\n";

    for (const auto &incl : visitor.includes) {
        mainSourceFile << "#include " << incl << std::endl;
    }

    mainSourceFile << planTupleDeclarations.str();
    mainSourceFile << result_wrapper;
    mainSourceFile << planLLVMDeclarations.str();

    // Main executable file: execute function
    mainSourceFile
            << format("extern \"C\" { result_type* execute(%s) { %s\n%s } }") %
                       input_args % planBody.str() % generatePlanDriver(sink);

    // Main executable file: free
    mainSourceFile << "extern \"C\" {"
                      "    void free_result(result_type *ptr) {"
                      "        if (ptr != nullptr && ptr->data != nullptr) {"
                      "            free(ptr->data);"
                      "            ptr->data = nullptr;"
                      "        }"
                      "        if (ptr != nullptr) {"
                      "            free(ptr);"
                      "        }"
                      "    }"
                      "}";

    // Header file
    std::ofstream headerFile(genDir + "execute.h");

    headerFile << return_type->ComputeDefinition();
    headerFile << result_wrapper;
    headerFile << format("result_type* execute(%s);\n") % input_args;
    headerFile << "void free_result(result_type*);";
}

void BackEnd::Compile(const uint64_t counter) {
    exec(("cd " + get_lib_path() +
          "/backend/gen && make LIB_ID=" + std::to_string(counter) + " -f " +
          get_lib_path() + "/backend/src/code_gen/cpp/Makefile -j")
                 .c_str());
}

}  // namespace cpp
}  // namespace code_gen
