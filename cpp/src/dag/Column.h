//
// Created by sabir on 26.08.17.
//

#ifndef DAG_COLUMN_H
#define DAG_COLUMN_H

#include <iostream>
#include <vector>

#include "dag/TupleField.h"

/**
 * part of the schema
 */
class Column {
public:
    static Column *makeColumn();

    void addField(TupleField *field);

    void addFields(const std::vector<TupleField *> &fields);

    bool operator==(const Column &other) { return id == other.id; }

    bool operator<(const Column &other) { return id < other.id; }

    std::string get_name() { return "c" + std::to_string(id); }

    static void delete_columns();

    std::vector<TupleField *> getFields();

private:
    Column() { id = column_counter++; }

    size_t id;
    std::vector<TupleField *> fields;
    static size_t column_counter;
    static std::vector<Column *> all_columns;
};

#endif  // DAG_COLUMN_H
