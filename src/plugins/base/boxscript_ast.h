#ifndef BOXSCRIPT_AST_H
#define BOXSCRIPT_AST_H

#include <boost/optional.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include "path.h"
#include "value.h"

namespace base {
    class BoxBuilder;
    class Expression;
}

namespace boxscript { namespace ast
{
    namespace x3 = boost::spirit::x3;

    struct Expression;
    struct Operand;

    typedef x3::forward_ast<Expression> GroupedExpression;

    struct Time : x3::position_tagged {
        int hour, minute, second;
        friend std::ostream& operator<<(std::ostream& os, const Time& x);
        base::Value value() const;
    };

    struct BareDate : x3::position_tagged {
        int day, month;
        friend std::ostream& operator<<(std::ostream& os, const BareDate& x);
        base::Value value() const;
    };

    struct Bool : x3::position_tagged {
        std::string stringValue;
        friend std::ostream& operator<<(std::ostream& os, const Bool& x);
        base::Value value() const;
    };

    struct Date : x3::position_tagged {
        int day, month, year;
        friend std::ostream& operator<<(std::ostream& os, const Date& x);
        base::Value value() const;
    };

    struct DateTime : x3::position_tagged {
        Date date;
        Time time;
        friend std::ostream& operator<<(std::ostream& os, const DateTime& x);
        base::Value value() const;
    };

    struct FunctionCall : x3::position_tagged {
        typedef x3::forward_ast<Expression> Argument;

        std::string name;
        std::vector<Argument> arguments;
        friend std::ostream& operator<<(std::ostream& os, const FunctionCall& x);
        void build(base::Expression *expression);
    };

    struct Path : x3::position_tagged {
        boost::optional<std::string> root;
        std::vector<std::string> objectNames;
        friend std::ostream& operator<<(std::ostream& os, const Path& x);
    };

    struct QuotedString : x3::position_tagged {
        std::string stringValue;
        friend std::ostream& operator<<(std::ostream& os, const QuotedString& x);
        base::Value value() const;
    };

    struct Reference : x3::position_tagged {
        Path path;
        std::string port;
        friend std::ostream& operator<<(std::ostream& os, const Reference& x);
        base::Path value() const;
    };

    #define TYPE_NAME(x,y) case x : s=#y; break

    struct Number : x3::variant<double, int>,
            x3::position_tagged
    {
        enum class Type{Double, Int};
        Type type() const {
            return static_cast<Type>(get().which());
        }
        std::string typeName() const {
            std::string s;
            switch (type()) {
                TYPE_NAME(Type::Double, Double);
                TYPE_NAME(Type::Int, Int);
            }
            return s;
        }
        using base_type::base_type;
        using base_type::operator=;
        friend std::ostream& operator<<(std::ostream& os, const Number& x);
        base::Value value() const;
    };

    struct Operand : x3::variant<DateTime, Date, BareDate, Time, Number,
                                 Reference, FunctionCall, Bool, QuotedString, GroupedExpression>,
            x3::position_tagged
    {
        enum class Type{DateTime, Date, BareDate, Time, Number,
                        Reference, FunctionCall, Bool, QuotedString, GroupedExpression};
        Type type() const {
            return static_cast<Type>(get().which());
        }
        std::string typeName() const {
            std::string s;
            switch (type()) {
                TYPE_NAME(Type::Number, Number);
                TYPE_NAME(Type::Date, Date);
                TYPE_NAME(Type::Time, Time);
                TYPE_NAME(Type::DateTime, DateTime);
                TYPE_NAME(Type::BareDate, BareDate);
                TYPE_NAME(Type::Reference, Reference);
                TYPE_NAME(Type::FunctionCall, FunctionCall);
                TYPE_NAME(Type::Bool, Bool);
                TYPE_NAME(Type::QuotedString, QuotedString);
                TYPE_NAME(Type::GroupedExpression, GroupedExpression);
            }
            return s;
        }
        using base_type::base_type;
        using base_type::operator=;
        friend std::ostream& operator<<(std::ostream& os, const Operand& x);
        void build(base::Expression *expression);
    };

    struct Operation : x3::position_tagged {
        std::string operator_;
        Operand operand;
        friend std::ostream& operator<<(std::ostream& os, const Operation& x);
        void build(base::Expression *expression);
    };

    struct Expression : x3::position_tagged {
        boost::optional<char> sign;
        Operand firstOperand;
        std::vector<Operation> operations;
        friend std::ostream& operator<<(std::ostream& os, const Expression& x);
        void build(base::BoxBuilder *builder);
        void build(base::Expression *expression);
    };

    struct Assignment : x3::position_tagged {
        char qualifier;
        std::string portName;
        char equals;
        Expression expression;
        friend std::ostream& operator<<(std::ostream& os, const Assignment& x);
        friend std::ostream& print     (std::ostream& os, const Assignment& x, int level);
        void build(base::BoxBuilder *builder);
    };

    struct Box;
    typedef boost::recursive_wrapper<Box> ChildBox;
    std::ostream& operator<<(std::ostream& os, const ChildBox& x);
    std::ostream& print     (std::ostream& os, const ChildBox& x, int level);

    struct Box : x3::position_tagged {
        std::string className, objectName;
        std::vector<Assignment> assignments;
        std::vector<ChildBox> children;
        friend std::ostream& operator<<(std::ostream& os, const Box& x);
        friend std::ostream& print     (std::ostream& os, const Box& x, int level);
        void build(base::BoxBuilder *builder);
    };

    struct boxscript : x3::position_tagged
    {
        Box root;
        friend std::ostream& operator<<(std::ostream& os, const boxscript& x);
        friend std::ostream& print     (std::ostream& os, const boxscript& x);
    };

}}

#endif
