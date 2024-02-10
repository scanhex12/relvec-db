#include "aggregate_function.h"

namespace shdb
{

namespace
{

enum SimpleAggregateFunctionType
{
    min,
    max,
    sum,
    avg
};

template <SimpleAggregateFunctionType AggregateFunctionType>
class SimpleAggregateFunction : public IAggregateFunction
{
public:
    explicit SimpleAggregateFunction(const std::vector<Type> & argument_types_)
        : IAggregateFunction(argument_types_), result_type(Type::int64)
    {
        throw std::runtime_error("Not implemented");
    }

    Type getResultType() override { return result_type; }

    size_t getStateSize() override { throw std::runtime_error("Not implemented"); }

    void create(AggregateDataPtr place) override
    {
        (void)(place);
        throw std::runtime_error("Not implemented");
    }

    void destroy(AggregateDataPtr place) override
    {
        (void)(place);
        throw std::runtime_error("Not implemented");
    }

    void add(AggregateDataPtr place, Row arguments) override
    {
        (void)(place);
        (void)(arguments);
        throw std::runtime_error("Not implemented");
    }

    Value getResult(AggregateDataPtr place) override
    {
        (void)(place);
        throw std::runtime_error("Not implemented");
    }

private:
    Type result_type;
};

}

AggregateFunctionPtr
AggregateFunctionFactory::getAggregateFunctionOrNull(const std::string & aggregate_function_name, const Types & argument_types)
{
    (void)(aggregate_function_name);
    (void)(argument_types);
    throw std::runtime_error("Not implemented");
}

AggregateFunctionPtr
AggregateFunctionFactory::getAggregateFunctionOrThrow(const std::string & aggregate_function_name, const Types & argument_types)
{
    (void)(aggregate_function_name);
    (void)(argument_types);
    throw std::runtime_error("Not implemented");
}

void AggregateFunctionFactory::registerAggregateFunction(
    const std::string & aggregate_function_name, AggregateFunctionCreateCallback create_callback)
{
    (void)(aggregate_function_name);
    (void)(create_callback);
    throw std::runtime_error("Not implemented");
}

void registerAggregateFunctions(AggregateFunctionFactory & aggregate_function_factory)
{
}

}
