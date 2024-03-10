#include "schema.h"
#include <sstream>

namespace shdb
{

std::string toString(Type type)
{
    switch (type)
    {
        case Type::boolean:
            return "boolean";
        case Type::uint64:
            return "uint64";
        case Type::int64:
            return "int64";
        case Type::varchar:
            return "varchar";
        case Type::string:
            return "string";
    }

    return {};
}

std::ostream & operator<<(std::ostream & stream, Type type)
{
    stream << toString(type);
    return stream;
}

std::string toString(const ColumnSchema & schema)
{
    std::string result = schema.name + " " + toString(schema.type);

    if (schema.length != 0)
    {
        result += '(';
        result += std::to_string(schema.length);
        result += ')';
    }

    return result;
}

std::ostream & operator<<(std::ostream & stream, const ColumnSchema & schema)
{
    stream << toString(schema);
    return stream;
}

std::string toString(const Schema & schema)
{
    std::string result;

    for (const auto & column : schema)
    {
        result += toString(column);
        result += ", ";
    }

    if (!result.empty())
    {
        result.pop_back();
        result.pop_back();
    }

    return result;
}

std::ostream & operator<<(std::ostream & stream, const Schema & schema)
{
    stream << toString(schema);
    return stream;
}

Schema deserializeSchema(const std::string& serializedSchema)
{
    Schema schema;
    std::istringstream iss(serializedSchema);
    std::string token;

    while (std::getline(iss, token, ','))
    {
        std::istringstream tokenStream(token);
        std::string columnName;
        std::string typeName;
        uint32_t length = 0;

        tokenStream >> columnName >> typeName;

        size_t pos = typeName.find('(');
        if (pos != std::string::npos)
        {
            length = std::stoul(typeName.substr(pos + 1, typeName.size() - pos - 2));
            typeName = typeName.substr(0, pos);
        }

        Type type;
        if (typeName == "boolean")
            type = Type::boolean;
        else if (typeName == "uint64")
            type = Type::uint64;
        else if (typeName == "int64")
            type = Type::int64;
        else if (typeName == "varchar")
            type = Type::varchar;
        else if (typeName == "string")
            type = Type::string;
        else
            throw std::runtime_error("Invalid type name");

        schema.emplace_back(columnName, type, length);
    }

    return schema;
}

}
