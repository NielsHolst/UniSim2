#include <iostream>
#include <boost/variant/get.hpp>
#include <base/box.h>
#include <base/expression.h>
#include <base/operator.h>
#include "exception_expectation.h"
#include "test_expression.h"

using boost::get;
using namespace base;

void TestExpression::testPush() {
    Expression expr;
    expr.push(17.5);
    expr.push(123);
    expr.push(2);
    expr.push(Operator::Multiply);
    expr.push(Operator::Add);

    QCOMPARE(Expression::type(expr.at(0)), Expression::Type::Value);
    QCOMPARE(Expression::type(expr.at(1)), Expression::Type::Value);
    QCOMPARE(Expression::type(expr.at(2)), Expression::Type::Value);
    QCOMPARE(Expression::type(expr.at(3)), Expression::Type::Operator);
    QCOMPARE(Expression::type(expr.at(4)), Expression::Type::Operator);
}

void TestExpression::testToPostfix() {
    bool excepted(false);
    Expression expr;
    expr.push(17.5);
    expr.push(Operator::Add);
    expr.push(123);
    expr.push(Operator::Multiply);
    expr.push(2);

    QCOMPARE(expr.originalAsString(), "17.5 + 123 * 2");
    try {
        expr.close();
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(expr.stackAsString(), "17.5 123 2 * +");

    expr.clear();
    expr.push(17.5);
    expr.push(Operator::Multiply);
    expr.push(123);
    expr.push(Operator::Add);
    expr.push(2);

    QCOMPARE(expr.originalAsString(), "17.5 * 123 + 2");
    try {
        expr.close();
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(expr.stackAsString(), "17.5 123 * 2 +");



}

void TestExpression::testEmpty() {
    bool excepted(false);
    Expression expr;
    try {
        expr.close();
    }
    UNEXPECTED_EXCEPTION;

    try {
        expr.evaluate();
    }
    EXPECTED_EXCEPTION_SHOWN;

    QVERIFY(expr.isEmpty());
}

void TestExpression::testSingle() {
    bool excepted(false);
    Box box("A", nullptr);
    Expression expr(&box);
    Value value;
    expr.push("abc");
    try {
        expr.close();
        value = expr.evaluate();
    }
    UNEXPECTED_EXCEPTION;

    QCOMPARE(value.as<QString>(), QString("abc"));
}

void TestExpression::testAddition() {
    bool excepted(false);
    using E    = Expression;
    using Type = Value::Type;
    Box box("A", nullptr);
    E e(&box);

    e.push(8);
    e.push(Operator::Add);
    e.push(9);
    e.push(Operator::Multiply);
    e.push(2);
    e.close();
    QCOMPARE(e.stackAsString(), "8 9 2 * +");

    Value result;
    try {
        result = e.evaluate();
        result.as<int>();
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(result.type(), Type::Int);
    QCOMPARE(result.as<int>(), 26);
}

void TestExpression::testResultType() {
    bool excepted(false);
    using E    = Expression;
    using Type = Value::Type;
    Box box("A", nullptr);
    E e(&box);

    e.push(8);
    e.push(Operator::Add);
    e.push(9.5);
    e.push(Operator::Multiply);
    e.push(2);
    QCOMPARE(e.stackAsString(), "8 + 9.5 * 2");
    e.close();
    QCOMPARE(e.stackAsString(), "8 9.5 2 * +");

    Value result;
    try {
        result = e.evaluate();
        result.as<double>();
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(result.type(), Type::Double);
    QCOMPARE(result.as<double>(), 27.);
}

void TestExpression::testNegation() {
    bool excepted(false);
    using E    = Expression;
    using Type = Value::Type;
    Box box("A", nullptr);
    E e(&box);

    e.push(Operator::Negate);
    e.push(8);
    e.push(Operator::Add);
    e.push(9);
    e.push(Operator::Multiply);
    e.push(2);
    e.close();
    QCOMPARE(e.stackAsString(), "8 -- 9 2 * +");

    Value result;
    try {
        result = e.evaluate();
        result.as<int>();
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(result.type(), Type::Int);
    QCOMPARE(result.as<int>(), 10);
}

void TestExpression::testExponentiation() {
    bool excepted(false);
    using E    = Expression;
    using Type = Value::Type;
    Box box("A", nullptr);
    E e(&box);

    e.push(8);
    e.push(Operator::Add);
    e.push(9);
    e.push(Operator::Exponentiate);
    e.push(2);
    e.push(Operator::Multiply);
    e.push(4);
    e.close();
    QCOMPARE(e.stackAsString(), "8 9 2 ^ 4 * +");

    Value result;
    try {
        result = e.evaluate();
        result.as<int>();
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(result.type(), Type::Int);
    QCOMPARE(result.as<int>(), 8 + 9*9*4);
}

void TestExpression::testFunctionCall() {
    bool excepted(false);
    using E     = Expression;
    using Func  = Expression::FunctionCall;
    Box box("A", nullptr);
    E e(&box);

    e.push(Func("sum", 3));
    e.push(8);
    e.push(Operator::Add);
    e.push(2);
    e.push(Operator::Multiply);
    e.push(3);
    e.push(Operator::Comma);
    e.push(13);
    e.push(Operator::Comma);
    e.push(14);
    e.push(Operator::Comma);
    e.push(Expression::FunctionCallEnd());
    e.push(Operator::Subtract);
    e.push(100);
    try {
        e.close();
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(e.stackAsString(), "8 2 3 * + , 13 , 14 , sum[3] 100 -");

    Value result;
    try {
        result = e.evaluate();
        result.as<int>();
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(result.type(), Value::Type::Int);
    QCOMPARE(result.as<int>(), 8 + 2 * 3 + 13 +14 - 100);
}
