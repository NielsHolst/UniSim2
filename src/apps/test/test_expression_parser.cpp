#include <iostream>
#include <string>
#include <base/boxscript_ast.h>         // defines << operators on AST
#include <base/boxscript_parser.h>      // defines the parser
#include <base/exception.h>
#include <base/expression.h>
#include "exception_expectation.h"
#include "test_expression_parser.h"

using namespace base;
using boxscript::parser::parse;
using boxscript::parser::Result;

void TestExpressionParser::testAstHandling() {
    bool excepted(false);
    Result ast;
    try {
        ast = parse("Box { &x = 7 + 17.5 }");
    }
    UNEXPECTED_EXCEPTION;

    // Show AST
//    using namespace ast;
//    std::cout << *r << std::endl;

    // Extract expression
    ast::Box root = ast->root;
    QCOMPARE(root.assignments.size(), (size_t) 1);
    ast::Assignment assignment = root.assignments.at(0);
    QCOMPARE(assignment.type(), ast::Assignment::Type::Expression);
    auto expression = boost::get<ast::Expression>(assignment.expression);
    ast::Operand op1 = expression.firstOperand;
    QCOMPARE(expression.operations.size(), (size_t) 1);
    ast::Operation operation = expression.operations.at(0);
    ast::Operand op2 = operation.operand;
    std::string operator_ = operation.operator_;

    QCOMPARE(op1.type(), ast::Operand::Type::Number);
    QCOMPARE(op2.type(), ast::Operand::Type::Number);
    QCOMPARE(operator_, std::string("+"));

    ast::Number num1 = boost::get<ast::Number>(op1),
                           num2 = boost::get<ast::Number>(op2);
    QCOMPARE(num1.type(), ast::Number::Type::Int);
    QCOMPARE(num2.type(), ast::Number::Type::Double);

    int    i = boost::get<int   >(num1);
    double x = boost::get<double>(num2);
    QCOMPARE(i, 7);
    QCOMPARE(x, 17.5);
}

void TestExpressionParser::testOperation() {
    bool excepted(false);
    base::Expression e;
    try {
        e = boxscript::parser::parseExpression("7 + 17.5");
    }
    UNEXPECTED_EXCEPTION;
    QString s = e.stackAsString();
    QCOMPARE(e.stackAsString(), "7.0 17.5 +");
}

void TestExpressionParser::testPathWithPort() {
    bool excepted(false);

    // Parse as full BoxScript
    Result ast;
    try {
        ast = parse("Box { &x = a/b/c[x] }");
    }
    UNEXPECTED_EXCEPTION;

    ast::Box root = ast->root;
    QCOMPARE(root.assignments.size(), (size_t) 1);
    ast::Assignment assignment = root.assignments.at(0);
    QCOMPARE(assignment.type(), ast::Assignment::Type::Expression);
    auto expression = boost::get<ast::Expression>(assignment.expression);
    ast::Operand op1 = expression.firstOperand;
    QCOMPARE(expression.operations.size(), (size_t) 0);
    QCOMPARE(op1.type(), ast::Operand::Type::Path);
    ast::Path astPath = boost::get<ast::Path>(op1);

    // Parse as expression only
    base::Expression e;
    try {
        e = boxscript::parser::parseExpression("a/b/c[x]");
    }
    UNEXPECTED_EXCEPTION;

    Expression::Stack elements = e.original();
    QCOMPARE(elements.size(), (size_t) 1);
    Expression::Element el = elements[0];
    QCOMPARE(Expression::type(el), Expression::Type::Path);
    Path path = std::get<Path>(el);
    auto alternatives = path.alternatives();
    QCOMPARE(alternatives.size(), (size_t) 1);
    auto alternative = alternatives.at(0);
    QVERIFY(!alternative.hasRoot());
    auto nodes = alternative.nodes();
    QCOMPARE(nodes.size(), (size_t) 3);
    QCOMPARE(nodes.at(0).objectName(), QString("a"));
    QCOMPARE(nodes.at(1).objectName(), QString("b"));
    QCOMPARE(nodes.at(2).objectName(), QString("c"));
    auto port = alternative.port();
    QVERIFY(port.has_value());
    QCOMPARE(port.value().name(), QString("x"));
}

void TestExpressionParser::testPathWithoutPort() {
    bool excepted(false);
    base::Expression e;
    try {
        e = boxscript::parser::parseExpression("a/b/c[DUMMY]");
    }
    UNEXPECTED_EXCEPTION;

    Expression::Stack elements = e.original();
    QCOMPARE(elements.size(), (size_t) 1);
    Expression::Element el = elements[0];
    QCOMPARE(Expression::type(el), Expression::Type::Path);
    Path path = std::get<Path>(el);
    auto alternatives = path.alternatives();
    QCOMPARE(alternatives.size(), (size_t) 1);
    auto alternative = alternatives.at(0);
    QVERIFY(!alternative.hasRoot());
    auto nodes = alternative.nodes();
    QCOMPARE(nodes.size(), (size_t) 3);
    QCOMPARE(nodes.at(0).objectName(), QString("a"));
    QCOMPARE(nodes.at(1).objectName(), QString("b"));
    QCOMPARE(nodes.at(2).objectName(), QString("c"));
    auto port = alternative.port();
    QVERIFY(!port.has_value());
}

