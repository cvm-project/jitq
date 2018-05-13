#include <fstream>
#include <iostream>
#include <string>

#include <boost/program_options.hpp>

#include "dag/DAG.h"
#include "dag/DAGCreation.hpp"
#include "optimize/Optimizer.h"

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    // Read command line parameters
    std::string input_file_name;
    std::string output_file_name;
    size_t opt_level = 0;

    po::options_description desc("Deserialize, optimize, and serialize a DAG");
    desc.add_options()                             //
            ("help", "Produce this help message")  //
            ("optimization-level,O",
             po::value<size_t>(&opt_level)->default_value(0),
             "Optimization level")  //
            ("input,i", po::value<std::string>(&input_file_name),
             "Path to input file")  //
            ("output,o", po::value<std::string>(&output_file_name),
             "Path to output file");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help") > 0) {
        std::cout << desc << std::endl;
        return 0;
    }

    // Open input and output files
    std::ifstream input_file(input_file_name);
    std::ifstream output_file(output_file_name);

    std::istream& input = vm.count("input") > 0 ? input_file : std::cin;
    std::ostream output(vm.count("output") > 0 ? output_file.rdbuf()
                                               : std::cout.rdbuf());

    // Read file content and parse
    std::unique_ptr<DAG> dag(parse_dag(&input));

    if (opt_level > 0) {
        // Optimize
        Optimizer opt;
        opt.run(dag.get());
    }

    // Convert back to JSON
    nlohmann::json json(dag);

    // Write to output
    json >> output;
}