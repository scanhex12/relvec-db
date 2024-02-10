#pragma once

#include "expression.h"
#include "row.h"
#include <stdexcept>


namespace shdb
{

using AggregateDataPtr = char *;

class IAggregateFunction
{
public:
    explicit IAggregateFunction(Types arguments_types_) : arguments_types(std::move(arguments_types_)) { }

    virtual ~IAggregateFunction() = default;

    virtual Type getResultType() = 0;

    virtual size_t getStateSize() = 0;

    virtual void create(AggregateDataPtr place) = 0;

    virtual void destroy(AggregateDataPtr place) = 0;

    virtual void add(AggregateDataPtr place, Row arguments) = 0;

    virtual Value getResult(AggregateDataPtr place) = 0;

    const Types arguments_types;
};

using AggregateFunctionPtr = std::shared_ptr<IAggregateFunction>;

using AggregateFunctionCreateCallback = std::function<AggregateFunctionPtr(const Types & argument_types)>;

class AggregateFunctionFactory
{
public:
    AggregateFunctionPtr getAggregateFunctionOrNull(const std::string & aggregate_function_name, const Types & argument_types);

    AggregateFunctionPtr getAggregateFunctionOrThrow(const std::string & aggregate_function_name, const Types & argument_types);

    void registerAggregateFunction(const std::string & aggregate_function_name, AggregateFunctionCreateCallback create_callback);

private:
    std::unordered_map<std::string, AggregateFunctionCreateCallback> aggregate_function_name_to_create_callback;
};

void registerAggregateFunctions(AggregateFunctionFactory & aggregate_function_factory);

}
