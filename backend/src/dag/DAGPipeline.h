#ifndef DAG_DAGPIPELINE_H
#define DAG_DAGPIPELINE_H

#include <json.hpp>

#include "DAGOperator.h"

class DAGPipeline : public DAGOperatorBase<DAGPipeline> {
public:
    constexpr static const char *kName = "pipeline";
    constexpr static size_t kNumInPorts = 0;
    constexpr static size_t kNumOutPorts = 1;

    size_t num_in_ports() const override { return num_inputs; }

    void to_json(nlohmann::json *json) const override;
    void from_json(const nlohmann::json &json) override;

    size_t num_inputs{};
};

#endif  // DAG_DAGPIPELINE_H
