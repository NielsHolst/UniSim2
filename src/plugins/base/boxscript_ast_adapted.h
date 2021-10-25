#ifndef BOXSCRIPT_AST_ADAPTED_H
#define BOXSCRIPT_AST_ADAPTED_H

#include "boxscript_ast.h"

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/std_pair.hpp>

BOOST_FUSION_ADAPT_STRUCT(
    boxscript::ast::boxscript, root
)
BOOST_FUSION_ADAPT_STRUCT(
    boxscript::ast::Assignment, qualifier, portName, equals, expression
)
BOOST_FUSION_ADAPT_STRUCT(
    boxscript::ast::BareDate, day, month
)
BOOST_FUSION_ADAPT_STRUCT(
    boxscript::ast::BareDateTime, date, time
)
BOOST_FUSION_ADAPT_STRUCT(
    boxscript::ast::Bool, stringValue
)
BOOST_FUSION_ADAPT_STRUCT(
    boxscript::ast::Box, className, objectName, assignments, children
)
BOOST_FUSION_ADAPT_STRUCT(
    boxscript::ast::Date, day, month, year
)
BOOST_FUSION_ADAPT_STRUCT(
    boxscript::ast::DateTime, date, time
)
BOOST_FUSION_ADAPT_STRUCT(
    boxscript::ast::Expression, sign, firstOperand, operations
)
BOOST_FUSION_ADAPT_STRUCT(
    boxscript::ast::FunctionCall, name, arguments
)
BOOST_FUSION_ADAPT_STRUCT(
    boxscript::ast::IfExpression, expressions
)
BOOST_FUSION_ADAPT_STRUCT(
    boxscript::ast::Operation, operator_, operand
)
BOOST_FUSION_ADAPT_STRUCT(
    boxscript::ast::QuotedString, stringValue
)
BOOST_FUSION_ADAPT_STRUCT(
    boxscript::ast::Reference, path, port
)
BOOST_FUSION_ADAPT_STRUCT(
    boxscript::ast::Time, hour, minute, second
)

#endif
