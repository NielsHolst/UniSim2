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
        ast = parse("Box { +x = 7 + 17.5 }");
    }
    UNEXPECTED_EXCEPTION;

    // Show AST
//    using namespace boxscript::ast;
//    std::cout << *r << std::endl;

    // Extract expression
    boxscript::ast::Box root = ast->root;
    QCOMPARE(root.assignments.size(), (size_t) 1);
    boxscript::ast::Assignment assignment = root.assignments.at(0);
    boxscript::ast::Expression expression = assignment.expression;
    boxscript::ast::Operand op1 = expression.firstOperand;
    QCOMPARE(expression.operations.size(), (size_t) 1);
    boxscript::ast::Operation operation = expression.operations.at(0);
    boxscript::ast::Operand op2 = operation.operand;
    std::string operator_ = operation.operator_;

    QCOMPARE(op1.type(), boxscript::ast::Operand::Type::Number);
    QCOMPARE(op2.type(), boxscript::ast::Operand::Type::Number);
    QCOMPARE(operator_, std::string("+"));

    boxscript::ast::Number num1 = boost::get<boxscript::ast::Number>(op1),
                           num2 = boost::get<boxscript::ast::Number>(op2);
    QCOMPARE(num1.type(), boxscript::ast::Number::Type::Int);
    QCOMPARE(num2.type(), boxscript::ast::Number::Type::Double);

    int    i = boost::get<int   >(num1);
    double x = boost::get<double>(num2);
    QCOMPARE(i, 7);
    QCOMPARE(x, 17.5);
}

void TestExpressionParser::testParser() {
    bool excepted(false);
    base::Expression e;
    try {
        e = boxscript::parser::parseExpression("7 + 17.5");
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(e.stackAsString(), "7.0 17.5 +");

}
