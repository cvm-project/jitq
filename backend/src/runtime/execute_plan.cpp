#include "execute_plan.hpp"

#include <cstring>
#include <vector>

#include "memory/values.hpp"
#include "utils/registry.hpp"
#include "values/json_parsing.hpp"

namespace runtime {

using PlanRegistry = utils::Registry<Plan, size_t>;

std::string ExecutePlan(const size_t plan_id, const std::string &inputs_str) {
    const auto inputs =
            runtime::values::ConvertFromJsonString(inputs_str.c_str());
    const auto plan = *(PlanRegistry::at(plan_id));
    const auto ret = plan(inputs);
    runtime::memory::Increment(ret);
    return runtime::values::ConvertToJsonString(ret);
}

size_t RegisterPlan(const Plan &plan) {
    auto const plan_id = PlanRegistry::num_objects();
    PlanRegistry::Register(plan_id, std::make_unique<Plan>(plan));
    return plan_id;
}

}  // namespace runtime
