#ifndef DAG_COLLECTION_ATTRIBUTE_ID_HPP
#define DAG_COLLECTION_ATTRIBUTE_ID_HPP

#include <iostream>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "field.hpp"

namespace dag {

/**
 * part of the schema
 */
class AttributeId : public std::enable_shared_from_this<AttributeId> {
public:
    static std::shared_ptr<AttributeId> MakeAttributeId();

    void AddField(collection::Field *field);

    void MoveFields(const std::shared_ptr<AttributeId> &other);
    void RemoveField(collection::Field *field);
    bool operator==(const AttributeId &other) { return id_ == other.id_; }

    bool operator<(const AttributeId &other) { return id_ < other.id_; }

    std::string name() { return "c" + std::to_string(id_); }

private:
    AttributeId() { id_ = column_counter_++; }

    size_t id_;
    std::unordered_set<collection::Field *> fields_{};
    static size_t column_counter_;
};
}  // namespace dag

#endif  // DAG_COLLECTION_ATTRIBUTE_ID_HPP