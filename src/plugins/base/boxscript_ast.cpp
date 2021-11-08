#include <iostream>
#include <sstream>
#include "boxscript_ast.h"

namespace boxscript { namespace ast
{
    inline std::string pad(int level) { return std::string(2*level, ' '); }

    inline std::string zpad(int i) {
        std::string s = std::to_string(i);
        return std::string(2 - s.length(), '0') + s;
    }

    std::ostream& operator<<(std::ostream& os, const Assignment& x) {
        return print(os, x, 0);
    }

    std::ostream& print(std::ostream& os, const Assignment& x, int level) {
        os << pad(level) << x.qualifier << x.portName;
        os << " " << x.equals << " " << x.expression << std::endl;
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const BareDate& x) {
        return os << x.day << "/" << x.month;
    }

    std::ostream& operator<<(std::ostream& os, const BareDateTime& x) {
        return os << x.date << "T" << x.time;
    }

    std::ostream& operator<<(std::ostream& os, const Bool& x) {
        return os << x.stringValue;
    }

    std::ostream& operator<<(std::ostream& os, const Box& x) {
        return print(os, x, 0);
    }

    std::ostream& print(std::ostream& os, const Box& x, int level) {
        os << pad(level) << x.className << " " << x.objectName << " {\n";
        for (auto y : x.assignments)
            print(os, y, level+1);
        for (auto y : x.children)
            print(os, y, level+1);
        os << pad(level) << "}\n";
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const boxscript& x) {
        return print(os, x.root, 0);
    }

    std::ostream& operator<<(std::ostream& os, const ChildBox& x) {
        return print(os, x, 0);
    }

    std::ostream& print(std::ostream& os, const ChildBox& x, int level) {
        return print(os, x.get(), level);
    }

    std::ostream& operator<<(std::ostream& os, const Date& x) {
        return os << x.day << "/" << x.month << "/" << x.year;
    }

    std::ostream& operator<<(std::ostream& os, const DateTime& x) {
        return os << x.date << "T" << x.time;
    }

    std::ostream& operator<<(std::ostream& os, const Expression& x) {
        if (x.sign.has_value() && x.sign.value()=='-')
            os << '-';
        os << x.firstOperand;
        for (const Operation &operation : x.operations)
            os << operation;
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const FunctionCall& x) {
        std::stringstream str;
        str << x.name << "(";
        for (auto argument : x.arguments)
            str << argument.get() << ", ";
        std::string s = str.str();
        return os << s.erase(s.length()-2) << ")" ;
    }

    std::ostream& operator<<(std::ostream& os, const Number& x) {
        return os << x.get();
    }

    std::ostream& operator<<(std::ostream& os, const Operand& x) {
        bool isGrouped = (x.type() == Operand::Type::GroupedExpression);
        if (isGrouped) os << "(";
        os << x.get();
        if (isGrouped) os << ")";
        return os;

    }

    std::ostream& operator<<(std::ostream& os, const Operation& x) {
        return os << x.operator_ << x.operand;
    }

    std::ostream& operator<<(std::ostream& os, const QuotedString& x) {
        return os << '"' << x.stringValue << '"';
    }

    std::ostream& operator<<(std::ostream& os, const Reference& x) {
        return os << x.path << "[" << x.port << "]";
    }

    std::ostream& operator<<(std::ostream& os, const Time& x) {
        return os << zpad(x.hour) << ":" << zpad(x.minute) << ":" << zpad(x.second);
    }



}}

