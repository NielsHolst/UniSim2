#ifndef BOXSCRIPT_AST_H
#define BOXSCRIPT_AST_H

#include <boost/optional.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>

#include <map>

namespace boxscript { namespace ast
{
    namespace x3 = boost::spirit::x3;

    struct Expression;
    struct Operand;
    struct Summary;

    typedef x3::forward_ast<Expression> GroupedExpression;

    struct Time : x3::position_tagged {
        int hour, minute, second;
        friend std::ostream& operator<<(std::ostream& os, const Time& x);
    };

    struct BareDate : x3::position_tagged {
        int day, month;
        friend std::ostream& operator<<(std::ostream& os, const BareDate& x);
    };

    struct BareDateTime : x3::position_tagged {
        BareDate date;
        Time time;
        friend std::ostream& operator<<(std::ostream& os, const BareDateTime& x);
    };

    struct Bool : x3::position_tagged {
        std::string stringValue;
        friend std::ostream& operator<<(std::ostream& os, const Bool& x);
    };

    struct Date : x3::position_tagged {
        int day, month, year;
        friend std::ostream& operator<<(std::ostream& os, const Date& x);
    };

    struct DateTime : x3::position_tagged {
        Date date;
        Time time;
        friend std::ostream& operator<<(std::ostream& os, const DateTime& x);
    };

    struct FunctionCall : x3::position_tagged {
        typedef x3::forward_ast<Operand> Argument;

        std::string name;
        Argument argument;
        friend std::ostream& operator<<(std::ostream& os, const FunctionCall& x);
    };

    struct QuotedString : x3::position_tagged {
        std::string stringValue;
        friend std::ostream& operator<<(std::ostream& os, const QuotedString& x);
    };

    struct Summary : x3::position_tagged{
        std::string stringValue;
        friend std::ostream& operator<<(std::ostream& os, const Summary& x);
    };

    struct Reference : x3::position_tagged {
        std::string path, port;
        boost::optional<Summary> summary;
        friend std::ostream& operator<<(std::ostream& os, const Reference& x);
    };

    #define TYPE_NAME(x,y) case x : s=#y; break
    struct Operand : x3::variant<DateTime, BareDateTime, Date, BareDate, Time, double,
                                 Reference, FunctionCall, Bool, QuotedString, GroupedExpression>,
            x3::position_tagged
    {
        enum class Type{DateTime=0, BareDateTime=1, Date=2, BareDate=3, Time=4, Double=5,
                        Reference=6, FunctionCall=7, Bool=8, QuotedString=9, GroupedExpression=10};
        Type type() const {
            return static_cast<Type>(get().which());
        }
        std::string typeName() const {
            std::string s;
            switch (type()) {
                TYPE_NAME(Type::Double, Double);
                TYPE_NAME(Type::Date, Date);
                TYPE_NAME(Type::Time, Time);
                TYPE_NAME(Type::DateTime, DateTime);
                TYPE_NAME(Type::BareDate, BareDate);
                TYPE_NAME(Type::BareDateTime, BareDateTime);
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
    };

    struct Operation : x3::position_tagged {
        char operator_;
        Operand operand;
        friend std::ostream& operator<<(std::ostream& os, const Operation& x);
    };

    struct Expression : x3::position_tagged {
        boost::optional<char> sign;
        Operand firstOperand;
        std::vector<Operation> operations;
        friend std::ostream& operator<<(std::ostream& os, const Expression& x);
    };

    struct Assignment : x3::position_tagged {
        char qualifier;
        std::string portName;
        boost::optional<Summary> summary;
        char equals;
        Expression expression;
        friend std::ostream& operator<<(std::ostream& os, const Assignment& x);
        friend std::ostream& print     (std::ostream& os, const Assignment& x, int level);
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
    };

    struct boxscript : x3::position_tagged
    {
        Box root;
        friend std::ostream& operator<<(std::ostream& os, const boxscript& x);
        friend std::ostream& print     (std::ostream& os, const boxscript& x);
    };

}}

#endif
