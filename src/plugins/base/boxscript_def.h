/*============================================================================
    Copyright (c) 2001-2015 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef BOXSCRIPT_DEF_H
#define BOXSCRIPT_DEF_H

#include "boxscript_ast.h"
#include "boxscript_ast_adapted.h"
#include "boxscript_error_handler.h"
#include "boxscript.h"

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
#pragma clang diagnostic ignored "-Wunneeded-internal-declaration"
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#pragma clang diagnostic ignored "-Woverloaded-shift-op-parentheses"

namespace boxscript { namespace parser
{
    namespace x3 = boost::spirit::x3;
    using x3::char_;
    using x3::double_;
    using x3::int_;
    using x3::digit;
    using x3::eol;
    using x3::eoi;
    using x3::lexeme;
    using x3::lit; // to ignore string

    ///////////////////////////////////////////////////////////////////////////
    // Rule IDs
    ///////////////////////////////////////////////////////////////////////////

    struct assignment_class;
    struct bare_date_class;
    struct bare_date_time_class;
    struct bool__class;
    struct box_class;
    struct date_class;
    struct date_time_class;
    struct dots_class;
    struct expression_class;
    struct function_call_class;
    struct grouped_expression_class;
    struct integer_class;
    struct joker_class;
    struct name_class;
    struct number_class;
    struct object_name_class;
    struct operand_class;
    struct operation_class;
    struct operator__class;
    struct path_class;
    struct port_class;
    struct port_prefix_class;
    struct qualified_name_class;
    struct quoted_string_class;
    struct reference_class;
    struct sign_class;
    struct time_class;

    ///////////////////////////////////////////////////////////////////////////
    // Rules
    ///////////////////////////////////////////////////////////////////////////

    x3::rule<assignment_class, ast::Assignment> const assignment = "assignment";
    x3::rule<bare_date_class, ast::BareDate> const bare_date = "bare date";
    x3::rule<bare_date_time_class, ast::BareDateTime> const bare_date_time = "bare date_time";
    x3::rule<bool__class, ast::Bool> const bool_ = "bool";
    x3::rule<box_class, ast::Box> const box = "box";
    x3::rule<date_class, ast::Date> const date = "date";
    x3::rule<date_time_class, ast::DateTime> const date_time = "date_time";
    x3::rule<dots_class, std::string> const dots = "dots";
    x3::rule<expression_class, ast::Expression> const expression = "expression";
    x3::rule<function_call_class, ast::FunctionCall> const function_call = "function call";
    x3::rule<grouped_expression_class, ast::Expression> const grouped_expression = "grouped expression";
    x3::rule<integer_class, int> const integer = "integer";
    x3::rule<joker_class, std::string> const joker = "joker";
    x3::rule<name_class, std::string> const name = "name";
    x3::rule<number_class, double> const number = "number";
    x3::rule<object_name_class, std::string> const object_name = "object name";
    x3::rule<operand_class, ast::Operand> const operand = "operand";
    x3::rule<operation_class, ast::Operation> const operation = "operation";
    x3::rule<operator__class, char> const operator_ = "operator";
    x3::rule<path_class, std::string> const path = "path";
    x3::rule<port_class, std::string> const port = "port";
    x3::rule<port_prefix_class, char> const port_prefix = "port prefix (. or +)";
    x3::rule<qualified_name_class, std::string> const qualified_name = "qualified_name";
    x3::rule<quoted_string_class, ast::QuotedString> const quoted_string = "quoted_string";
    x3::rule<reference_class, ast::Reference> const reference = "reference";
    x3::rule<sign_class, char> const sign = "sign";
    x3::rule<time_class, ast::Time> const time = "time";
    boxscript_type const boxscript = "boxscript";

    ///////////////////////////////////////////////////////////////////////////
    // Grammar
    ///////////////////////////////////////////////////////////////////////////

    using boost::fusion::at_c;
    auto const dm = [](auto& ctx) {
        _val(ctx).day = at_c<0>(_attr(ctx));
        _val(ctx).month = at_c<1>(_attr(ctx));
    };

    auto const md = [](auto& ctx) {
        _val(ctx).day = at_c<1>(_attr(ctx));
        _val(ctx).month = at_c<0>(_attr(ctx));
    };

    auto const dmy_ymd = [](auto& ctx) {
        int i = at_c<0>(_attr(ctx)),
            j = at_c<1>(_attr(ctx)),
            k = at_c<2>(_attr(ctx));
        if (i<100) {
            _val(ctx).day = i;
            _val(ctx).month = j;
            _val(ctx).year = k;
        }
        else {
            _val(ctx).day = k;
            _val(ctx).month = j;
            _val(ctx).year = i;
        }
    };

    auto const mdy = [](auto& ctx) {
        int i = at_c<0>(_attr(ctx)),
            j = at_c<1>(_attr(ctx)),
            k = at_c<2>(_attr(ctx));
        _val(ctx).month = i;
        _val(ctx).day = j;
        _val(ctx).year = k;
    };

    auto const pipe_summary = [](auto& ctx) {
        std::string s = _attr(ctx);
        _val(ctx).stringValue = std::string("|") + s;
    };

    auto const assignment_def = (port_prefix > name) >> (char_('=')|char_('~')) > expression;
    auto const bare_date_def = (integer >> '/' >> integer) [dm] |
                        ('/' >> integer >> '/' >> integer) [md];
    auto const bare_date_time_def = bare_date >> (lit("T")|lit(" ")) >> time;
    auto const bool__def = lexeme[x3::string("TRUE") | x3::string("FALSE")];
    auto const box_def = qualified_name >> -name >> '{' >> *assignment >> *box > '}';
    auto const boxscript_def = box;
    auto const date_def = (integer >> '/' >> integer >> '/' > integer) [dmy_ymd] |
                          (integer >> '.' >> integer >> '.' > integer) [dmy_ymd] |
                          (integer >> '-' >> integer >> '-' > integer) [dmy_ymd] |
                   ('/' >> integer >> '/' >> integer >> '/' > integer) [mdy];
    auto const date_time_def = date >> lit("T") >> time;
    auto const dots_def = lexeme[x3::repeat(1,3)[x3::string(".")]];
    auto const expression_def = -sign >> operand >> *operation;
    auto const function_call_def = name >> '(' >> expression % ',' > ')';
    auto const grouped_expression_def = '(' >> expression > ')';
    auto const integer_def = int_;
    auto const joker_def = lexeme[x3::string("*")];
    auto const name_def = lexeme[char_("a-zA-Z") >> *char_("a-zA-Z0-9_")];
    auto const number_def = double_;
    auto const object_name_def = qualified_name | joker | dots;
    auto const operand_def = date_time | bare_date_time | date | bare_date | time | number |
                             reference | function_call | bool_ | quoted_string | grouped_expression;
    auto const operation_def = operator_ >> operand;
    auto const operator__def = char_('+')|char_('-')|char_('*')|char_('/')|char_('^')|char_('|');
    auto const path_def = -char_('/') >> object_name >> *(char_('/') >> object_name);
    auto const port_def = lit('[') >> (name | joker) > lit(']');
    auto const port_prefix_def = char_('.')|char_('+');
    auto const qualified_name_def = lexeme[name >> -(x3::string("::") > object_name)];
    auto const quoted_string_def = lexeme['"' >> *(char_ - '"') >> '"'];
    auto const reference_def = path >> port;
    auto const sign_def = char_("+")|char_("-");
    auto const time_def = integer >> ':' > integer >> -(':' > integer);

    BOOST_SPIRIT_DEFINE(
                assignment, bare_date, bare_date_time, bool_, box, date, date_time,
                dots, expression, function_call, grouped_expression, integer, joker,
                name, number, object_name, operand, operation, operator_,
                path, port, port_prefix, qualified_name, quoted_string, reference, sign,
                time, boxscript
                )

    ///////////////////////////////////////////////////////////////////////////
    // Annotation and Error handling
    ///////////////////////////////////////////////////////////////////////////

    // We want these to be annotated with the iterator position.
    struct assignment_class : x3::annotate_on_success {};
    struct bare_date_class : x3::annotate_on_success {};
    struct bare_date_time_class : x3::annotate_on_success {};
    struct bool__class : x3::annotate_on_success {};
    struct box_class : x3::annotate_on_success {};
    struct date_class : x3::annotate_on_success {};
    struct date_time_class : x3::annotate_on_success {};
    struct dots_class : x3::annotate_on_success {};
    struct expression_class : x3::annotate_on_success {};
    struct function_call_class : x3::annotate_on_success {};
    struct grouped_expression_class : x3::annotate_on_success {};
    struct integer_class : x3::annotate_on_success {};
    struct joker_class : x3::annotate_on_success {};
    struct name_class : x3::annotate_on_success {};
    struct number_class : x3::annotate_on_success {};
    struct object_name_class : x3::annotate_on_success {};
    struct operand_class : x3::annotate_on_success {};
    struct operation_class : x3::annotate_on_success {};
    struct operator__class : x3::annotate_on_success {};
    struct path_class : x3::annotate_on_success {};
    struct port_class : x3::annotate_on_success {};
    struct port_prefix_class : x3::annotate_on_success {};
    struct qualified_name_class : x3::annotate_on_success {};
    struct quoted_string_class : x3::annotate_on_success {};
    struct reference_class : x3::annotate_on_success {};
    struct sign_class : x3::annotate_on_success {};
    struct time_class : x3::annotate_on_success {};


    // We want error-handling only for the start (outermost) boxscript
    // boxscript is the same as boxscript_inner but without error-handling (see error_handler.hpp)
    struct boxscript_class : x3::annotate_on_success, error_handler_base {};
}}

namespace boxscript
{
    parser::boxscript_type const& boxscript()
    {
        return parser::boxscript;
    }
}

#pragma clang diagnostic push
#endif
