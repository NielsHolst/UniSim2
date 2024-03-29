#include <iostream>
#include <memory>
#include <sstream>
#include <utility>
#include <QFile>
#include <boost/spirit/home/x3.hpp>
#include <base/boxscript.h>
#include <base/boxscript_ast.h>
#include <base/boxscript_config.h>
#include <base/boxscript_def.h>
#include <base/boxscript_parser.h>
#include <base/exception.h>
#include "exception_expectation.h"
#include "input_file_path.h"
#include "test_box_script_x3.h"

using namespace std;
using namespace base;
namespace x3 = boost::spirit::x3;

typedef boxscript::parser::Result ParseResult;

ParseResult parse(QString filePath) {
    // Read source
    QFile file;
    openInputFile(file, filePath);
    std::string source = file.readAll().toStdString();
    // Parse
    return boxscript::parser::parse(source, filePath.toStdString());
}

bool compare(QString filePath, ParseResult result) {
    // Read source
    QFile file;
    openInputFile(file, filePath);
    QString source = file.readAll();

    std::stringstream str;
    str << *result;
    QString parsed = QString::fromStdString(str.str());
    bool ok =  (source.simplified() == parsed.simplified());
    if (!ok) {
        cout << qPrintable(parsed)
             << std::endl << qPrintable(source.simplified())
             << std::endl << qPrintable(parsed.simplified())
             << std::endl;
    }
    return ok;
}

template <class Result, class Rule>
Result parseRule(std::string source, Rule rule) {
    using boxscript::parser::iterator_type;
    iterator_type iter(source.begin());
    iterator_type const end(source.end());
    Result result;
    bool success = phrase_parse(iter, end, rule, x3::ascii::space, result);
    if (!success)
        ThrowException("Parse unsuccessful").value(source);
    return result;
}

void TestBoxScriptX3::testNumberDef() {
    double result;
    bool excepted(false);
    try {
        result = parseRule<double>("3.1412", boxscript::parser::number_def);
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(result, 3.1412);
}

void TestBoxScriptX3::testNameDef() {
    std::string result;
    bool excepted(false);
    try {
        result = parseRule<std::string>("A38", boxscript::parser::name_def);
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(result, "A38");
}

void TestBoxScriptX3::testJokerDef() {
    std::string result;
    bool excepted(false);
    try {
        result = parseRule<std::string>("*", boxscript::parser::joker_def);
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(result, "*");
}

void TestBoxScriptX3::testPortDef() {
    std::string result;
    bool excepted(false);
    try {
        result = parseRule<std::string>("abc", boxscript::parser::name_def | boxscript::parser::joker_def);
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(result, "abc");
}

void TestBoxScriptX3::testComments() {
    ParseResult result;
    bool excepted(false);
    try {
        result = parse("box_script/comments.box");
    }
    UNEXPECTED_EXCEPTION;
    QVERIFY(compare("box_script/comments_parsed.box", result));
}

void TestBoxScriptX3::testDate() {
    ParseResult result;
    bool excepted(false);
    try {
        result = parse("box_script/date.box");
    }
    UNEXPECTED_EXCEPTION;
    QVERIFY(compare("box_script/date_parsed.box", result));
}

void TestBoxScriptX3::testBareDate() {
    ParseResult result;
    bool excepted(false);
    try {
        result = parse("box_script/bare_date.box");
    }
    UNEXPECTED_EXCEPTION;
    QVERIFY(compare("box_script/bare_date_parsed.box", result));
}

void TestBoxScriptX3::testBareDateTime() {
    ParseResult result;
    bool excepted(false);
    try {
        result = parse("box_script/bare_date_time.box");
    }
    UNEXPECTED_EXCEPTION;
    QVERIFY(compare("box_script/bare_date_time_parsed.box", result));
}

void TestBoxScriptX3::testBool() {
    ParseResult result;
    bool excepted(false);
    try {
        result = parse("box_script/bool.box");
    }
    UNEXPECTED_EXCEPTION;
    QVERIFY(compare("box_script/bool.box", result));
}

void TestBoxScriptX3::testReference() {
    ParseResult result;
    bool excepted(false);
    try {
        result = parse("box_script/reference.box");
    }
    UNEXPECTED_EXCEPTION;
    QVERIFY(compare("box_script/reference_cleaned.box", result));
}

void TestBoxScriptX3::testDotsError() {
    ParseResult result;
    bool excepted(false);
    try {
        result = parse("box_script/dots_error.box");
    }
    EXPECTED_EXCEPTION;
}

void TestBoxScriptX3::testQuotedString() {
    ParseResult result;
    bool excepted(false);
    try {
        result = parse("box_script/quoted_string.box");
    }
    UNEXPECTED_EXCEPTION;
    QVERIFY(compare("box_script/quoted_string.box", result));
}

void TestBoxScriptX3::testFunctionCall() {
    ParseResult result;
    bool excepted(false);
    try {
        result = parse("box_script/function_call.box");
    }
    UNEXPECTED_EXCEPTION;
    QVERIFY(compare("box_script/function_call.box", result));
}

void TestBoxScriptX3::testBox() {
    ParseResult result;
    bool excepted(false);
    try {
        result = parse("box_script/box.box");
    }
    UNEXPECTED_EXCEPTION;
    QVERIFY(compare("box_script/box.box", result));
}

void TestBoxScriptX3::testOperator() {
    ParseResult result;
    bool excepted(false);
    try {
        result = parse("box_script/operator.box");
    }
    UNEXPECTED_EXCEPTION;
    QVERIFY(compare("box_script/operator.box", result));
}

void TestBoxScriptX3::testTooMuch() {
    ParseResult result;
    bool excepted(false);
    try {
        result = parse("box_script/too_much.box");
    }
    EXPECTED_EXCEPTION;
}

void TestBoxScriptX3::testNumber() {
    ParseResult result;
    bool excepted(false);
    try {
        result = parse("box_script/number.box");
    }
    UNEXPECTED_EXCEPTION;
    QVERIFY(compare("box_script/number.box", result));

    boxscript::ast::Box root = result->root;
    QCOMPARE(root.className, "Simulation");
    QCOMPARE(root.objectName, "sim");

    std::vector<boxscript::ast::Assignment> ass = root.assignments;
    QCOMPARE(ass.at(0).portName, "steps");
    QCOMPARE(ass.at(1).portName, "a");

    boxscript::ast::Operand op0 = ass.at(0).expression.firstOperand,
                            op1 = ass.at(1).expression.firstOperand;
    QCOMPARE(op0.type(), boxscript::ast::Operand::Type::Number);
    QCOMPARE(op1.type(), boxscript::ast::Operand::Type::Number);

    boxscript::ast::Number num0 = boost::get<boxscript::ast::Number>(op0),
                           num1 = boost::get<boxscript::ast::Number>(op1);
    QCOMPARE(num0.type(), boxscript::ast::Number::Type::Int);
    QCOMPARE(num1.type(), boxscript::ast::Number::Type::Double);


}
