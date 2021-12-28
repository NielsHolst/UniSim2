#include <iostream>
#include <sstream>
#include <QSet>
#include <QString>
#include <QDate>
#include <QDateTime>
#include <QTime>
#include "bare_date.h"
#include "box_builder.h"
#include "boxscript_ast.h"
#include "expression.h"
#include "operator.h"

//        using Type = Operand::Type;
//        static QSet<Operand::Type> constants {
//            Type::DateTime,
//            Type::Date,
//            Type::BareDate,
//            Type::Time,
//            Type::Number,
//            Type::Bool,
//            Type::QuotedString
//        };

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
        if (x.sign.has_value() && x.sign.value()!='+')
            os << x.sign.value();
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

    std::ostream& operator<<(std::ostream& os, const Path& x) {
        if (x.root.has_value())
            os << *x.root;
        for (auto objName = x.objectNames.begin(); objName != x.objectNames.end(); ++objName) {
            if (objName != x.objectNames.begin())
                os << "/";
            os << *objName;
        }
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const QuotedString& x) {
        return os << '"' << x.stringValue << '"';
    }

    std::ostream& operator<<(std::ostream& os, const Reference& x) {
        return os << x.path << "[" << x.port << "]";
    }

    std::ostream& operator<<(std::ostream& os, const ReferenceUnion& x) {
        for (auto ref = x.references.begin(); ref != x.references.end(); ++ref) {
            if (ref != x.references.begin())
                os << "|";
            os << *ref;
        }
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const Time& x) {
        return os << zpad(x.hour) << ":" << zpad(x.minute) << ":" << zpad(x.second);
    }

    inline QString str(const std::string &s) {
        return QString::fromStdString(s);
    }

    base::Value Time::value() const {
        return QTime(hour, minute, second);
    }

    base::Value BareDate::value() const {
        return base::BareDate(month, day);
    }

    base::Value Bool::value() const {
        return (stringValue == "TRUE");
    }

    base::Value Date::value() const {
        return QDate(year, month, day);
    }

    base::Value DateTime::value() const {
        return QDateTime(QDate(date.year, date.month, date.day),
                         QTime(time.hour, time.minute, time.second),
                         Qt::UTC);
    }

    void FunctionCall::build(base::Expression *expression) {
        auto f = base::Expression::FunctionCall(str(name), arguments.size());
        expression->push(f);
        for (auto arg : arguments)
            arg.get().build(expression);
    }

    base::Value QuotedString::value() const {
        return QString::fromStdString(stringValue);
    }

    base::Path ReferenceUnion::value() const {
        std::stringstream str;
        str << *this;
        return QString::fromStdString(str.str());
    }

    base::Value Number::value() const {
        using boost::get;
        return (type()==Type::Int) ? get<int>(*this) : get<double>(*this);
    }

    void Operand::build(base::Expression *expression) {
        using boost::get;
        switch (type()) {
        case Type::DateTime:
            expression->push(get<DateTime    >(*this).value());  break;
        case Type::Date:
            expression->push(get<Date        >(*this).value()); break;
        case Type::BareDate:
            expression->push(get<BareDate    >(*this).value());  break;
        case Type::Time:
            expression->push(get<Time        >(*this).value()); break;
        case Type::Number:
            expression->push(get<Number      >(*this).value()); break;
        case Type::ReferenceUnion:
            expression->push(get<ReferenceUnion>(*this).value()); break;
        case Type::FunctionCall:
                             get<FunctionCall>(*this).build(expression); break;
        case Type::Bool:
            expression->push(get<Bool        >(*this).value()); break;
        case Type::QuotedString:
            expression->push(get<QuotedString>(*this).value()); break;
        case Type::GroupedExpression:
            expression->push(base::Parenthesis::Left);
                             get<GroupedExpression>(*this).get().build(expression);
            expression->push(base::Parenthesis::Right);
            break;
        }
    }

    void Operation::build(base::Expression *expression) {
        expression->push( base::lookupOperator(QString::fromStdString(operator_)) );
        operand.build(expression);
    }

    void Expression::build(base::BoxBuilder *builder) {
        base::Expression expression;
        build(&expression);
        expression.close();
        builder->equals(expression);
    }

    void Expression::build(base::Expression *e) {
        if (sign.has_value()) {
            if (sign.value()=='-')
                e->push(base::Operator::Negate);
            else if (sign.value()=='!')
                e->push(base::Operator::Not);
        }
        firstOperand.build(e);
        for (auto op : operations)
            op.build(e);
    }

    void Assignment::build(base::BoxBuilder *builder) {
        bool isAuxPort = (qualifier == '+');
        if (isAuxPort)
            builder->aux(str(portName));
        else
            builder->port(str(portName));
        // Deal with assignment operator (=~) here...
        expression.build(builder);
    }

    void Box::build(base::BoxBuilder *builder) {
        builder
        ->box(str(className))
        .name(str(objectName));
        for (auto assignment : assignments)
            assignment.build(builder);
        for (auto child : children)
            child.get().build(builder);
    }


}}

