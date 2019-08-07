#ifndef DAG_OPERATORS_ROW_SCAN_HPP
#define DAG_OPERATORS_ROW_SCAN_HPP

#include <nlohmann/json.hpp>

#include "operator.hpp"

class DAGRowScan : public DAGOperator {
    JITQ_DAGOPERATOR(DAGRowScan, "row_scan");

public:
    size_t num_in_ports() const override { return 1; }
    size_t num_out_ports() const override { return 1; }

    void to_json(nlohmann::json *json) const override;
    void from_json(const nlohmann::json &json) override;

    bool add_index = false;
};

#endif  // DAG_OPERATORS_ROW_SCAN_HPP