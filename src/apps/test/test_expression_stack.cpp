#include <iostream>
#include <boost/variant/get.hpp>
#include <base/expression_operation.h>
#include <base/expression_stack.h>
#include "exception_expectation.h"
#include "test_expression_stack.h"

using boost::get;
using namespace expression;

void TestExpressionStack::testPush() {
    expression::Stack stack;
    Operand a = 17.5,
            b = 123,
            c = 2;
    stack.push(a);
    stack.push(b);
    stack.push(c);
    stack.push(Operator::Multiplication);
    stack.push(Operator::Addition);

    QCOMPARE(stack.at(0).type(), Element::Type::Operand);
    QCOMPARE(stack.at(1).type(), Element::Type::Operand);
    QCOMPARE(stack.at(2).type(), Element::Type::Operand);
    QCOMPARE(stack.at(3).type(), Element::Type::Operator);
    QCOMPARE(stack.at(4).type(), Element::Type::Operator);

    Operand op0 = get<Operand>(stack.at(0)),
            op1 = get<Operand>(stack.at(1)),
            op2 = get<Operand>(stack.at(2));

    Operand product = multiply(op1, op2),
            sum = add(product, op0);

    QCOMPARE(product.type(), Operand::Type::Int);
    QCOMPARE(sum.type(), Operand::Type::Double);

    QCOMPARE(get<int>(product), 246);
    QCOMPARE(get<double>(sum), 263.5);
}

void TestExpressionStack::testReduce() {
    expression::Stack stack;
    Operand a = 17.5,
            b = 123,
            c = 2,
            result;
    stack.push(a);
    stack.push(b);
    stack.push(c);
    stack.push(Operator::Multiplication);
    stack.push(Operator::Addition);

    stack.evaluate();
    result = stack.value();
    QCOMPARE(result.type(), Operand::Type::Double);
    QCOMPARE(get<double>(result), 263.5);
}

void TestExpressionStack::testToPostfix() {
    expression::Stack stack;
    Operand a = 17.5,
            b = 123,
            c = 2,
            result;
    stack.push(a);
    stack.push(Operator::Addition);
    stack.push(b);
    stack.push(Operator::Multiplication);
    stack.push(c);

    QCOMPARE(stack.toString(), "17.5 + 123 * 2");
    bool excepted(false);
    try {
        stack.toPostfix();
    }
    UNEXPECTED_EXCEPTION;

    QCOMPARE(stack.toString(), "17.5 123 2 * +");

}
