#include "apply_visitor.hpp"

#include <type_traits>

#include <boost/function_output_iterator.hpp>
#include <boost/graph/reverse_graph.hpp>
#include <boost/graph/topological_sort.hpp>

#include "dag/DAG.h"
#include "dag/DAGOperator.h"

// NOLINTNEXTLINE google-build-using-namespace
using namespace std::placeholders;

namespace dag {
namespace utils {

template <bool kReverseGraph, bool kDoRecursion>
void ApplyInTopologicalOrderImpl(DAG *const dag,
                                 const RecursiveDagVisitor &on_entry_visitor,
                                 const RecursiveDagVisitor &on_exit_visitor) {
    const auto graph = kReverseGraph ? boost::make_reverse_graph(dag->graph())
                                     : dag->graph();
    boost::topological_sort(
            graph, boost::make_function_output_iterator([&](const auto v) {
                const auto op = dag->to_operator(v);
                on_entry_visitor(op, dag);
                if (kDoRecursion) {
                    if (dag->has_inner_dag(op)) {
                        auto const inner_dag = dag->inner_dag(op);
                        ApplyInTopologicalOrderImpl<kReverseGraph,
                                                    kDoRecursion>(
                                inner_dag, on_entry_visitor, on_exit_visitor);
                    }
                    on_exit_visitor(op, dag);
                }
            }));
}

template <bool kReverseGraph, bool kDoRecursion>
void ApplyInTopologicalOrderImpl(
        const DAG *const dag, const RecursiveConstDagVisitor &on_entry_visitor,
        const RecursiveConstDagVisitor &on_exit_visitor) {
    ApplyInTopologicalOrderImpl<kReverseGraph, kDoRecursion>(
            // Cast away const qualifier
            const_cast<DAG *>(dag),
            [&](DAGOperator *op, DAG *const dag) {
                // OK -- we add it back when the visitor is called
                on_entry_visitor(op, const_cast<const DAG *>(dag));
            },
            [&](DAGOperator *op, DAG *const dag) {
                // OK -- we add it back when the visitor is called
                on_exit_visitor(op, const_cast<const DAG *>(dag));
            });
}

void ApplyInTopologicalOrder(DAG *const dag, const DagVisitor &visitor) {
    ApplyInTopologicalOrderImpl<false, false>(dag, std::bind(visitor, _1),
                                              [](...) {});
}

void ApplyInTopologicalOrder(const DAG *const dag, const DagVisitor &visitor) {
    ApplyInTopologicalOrderImpl<false, false>(dag, std::bind(visitor, _1),
                                              [](...) {});
}

void ApplyInTopologicalOrderRecursively(
        DAG *dag, const RecursiveDagVisitor &on_entry_visitor,
        const RecursiveDagVisitor &on_exit_visitor) {
    ApplyInTopologicalOrderImpl<false, true>(dag, on_entry_visitor,
                                             on_exit_visitor);
}

void ApplyInTopologicalOrderRecursively(
        const DAG *dag, const RecursiveConstDagVisitor &on_entry_visitor,
        const RecursiveConstDagVisitor &on_exit_visitor) {
    ApplyInTopologicalOrderImpl<false, true>(dag, on_entry_visitor,
                                             on_exit_visitor);
}

void ApplyInReverseTopologicalOrder(DAG *const dag, const DagVisitor &visitor) {
    ApplyInTopologicalOrderImpl<true, false>(dag, std::bind(visitor, _1),
                                             [](...) {});
}

void ApplyInReverseTopologicalOrder(const DAG *const dag,
                                    const DagVisitor &visitor) {
    ApplyInTopologicalOrderImpl<true, false>(dag, std::bind(visitor, _1),
                                             [](...) {});
}

void ApplyInReverseTopologicalOrderRecursively(
        DAG *dag, const RecursiveDagVisitor &on_entry_visitor,
        const RecursiveDagVisitor &on_exit_visitor) {
    ApplyInTopologicalOrderImpl<true, true>(dag, on_entry_visitor,
                                            on_exit_visitor);
}

void ApplyInReverseTopologicalOrderRecursively(
        const DAG *dag, const RecursiveConstDagVisitor &on_entry_visitor,
        const RecursiveConstDagVisitor &on_exit_visitor) {
    ApplyInTopologicalOrderImpl<true, true>(dag, on_entry_visitor,
                                            on_exit_visitor);
}

}  // namespace utils
}  // namespace dag
