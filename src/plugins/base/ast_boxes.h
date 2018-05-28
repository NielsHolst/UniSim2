#ifndef BASE_AST_BOXES_H
#define BASE_AST_BOXES_H
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/variant.hpp>
#include "ast_common.h"

#include <iostream>
#include <string>

namespace fusion = boost::fusion;
namespace phoenix = boost::phoenix;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::iso8859_1; // ascii;
using ascii::char_;
using phoenix::construct;
using phoenix::val;
using qi::lexeme;

BOOST_FUSION_ADAPT_STRUCT(
    ast::Node,
    (std::string, className)
    (std::string, objectName)
    (std::vector<ast::Parameter>, parameters)
    (std::vector<ast::CompositeNode>, children)
)


BOOST_FUSION_ADAPT_STRUCT(
    ast::ParameterWithAttribute,
    (std::string, type)
    (std::string, name)
    (std::string, attribute)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast::Parameter,
    (ast::ParameterWithAttribute, attributedName)
    (std::string, value)
)

#define RULE_NAME(X) X.name(#X)

namespace ast {

template <typename Iterator>
struct comment_skipper : public qi::grammar<Iterator>
{
    qi::rule<Iterator> _skip;

    comment_skipper()
        : comment_skipper::base_type(_skip)
    {
        _skip = ascii::space | ("//" >> *(char_ - qi::eol) >> qi::eol);
    }
};

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-shift-op-parentheses"

template <typename Iterator, typename Skipper = comment_skipper<Iterator>>
struct node_grammar : public qi::grammar<Iterator, Skipper, Node()>
{
    qi::rule<Iterator, Skipper, Node()> node;
    qi::rule<Iterator, Skipper, std::string()>
            class_name, object_name, name, joker, value, transform,
            unquoted_value, unquoted_value_item, quoted_value, list_value,
            trailing_list, unconditional_value, conditional_value;
    qi::rule<Iterator, Skipper, ParameterWithAttribute()> attributed_name;
    qi::rule<Iterator, Skipper, Parameter()> parameter;

    std::stringstream _error;

    node_grammar() : node_grammar::base_type(node) {
        // A name has C++ identifier style
        name %= lexeme[char_("a-zA-Z_") >> *char_("a-zA-Z0-9_")];
        // A joker is just a star
        joker %= lexeme[char_("*")];
        // classes and objects have a name;
        // a class name may be qualified by a namespace name
        class_name %= name >> -(char_(':') > char_(':') > name);
        object_name %= name;
        // An unconditional value may be quoted or unquoted or a list of values
        unconditional_value %= (quoted_value | unquoted_value | list_value);
        // A value may be unconditional or conditional an may be followed by a trailing list
        value %= (unconditional_value | conditional_value) >> -trailing_list;
        // A quoted value is a string of any characters, except apostrophes;
        // bracing apostrophes are kept in the string
        quoted_value %= lexeme[char_('"') >> *(char_ - '"') > char_('"')];
        // An unquoted value item is for numbers, dates, times, booleans and path expressions
        unquoted_value_item %= lexeme[+(char_ - char_(" \t\n\"{}()[]@?:"))]
                >> -(char_('[') > (joker|name) > char_(']'));
        // A trailing list is preceeded by @
        trailing_list %= char_('@') > list_value;
        // An unquoted value is one or more items maybe followed by a list
        unquoted_value %= unquoted_value_item >> *(char_("|") > unquoted_value_item);
        // A list value is captured; keeping parentheses and everything inside
        list_value %= lexeme[char_('(') >> *(char_ - ')') > char_(')')];
        // A conditional value is preceeded by ?
        conditional_value %= char_('?') > unquoted_value > char_(':') > unconditional_value;
        // A name with optional attribute
        transform = name;
        attributed_name %= char_("\\.+") >> name >> -('|' > transform);
        // A parameter has a name, maybe with attributes, and a value and maybe a distribution
        parameter %= attributed_name > '=' >> value;
        // A node has a class name, maybe an object name, maybe some parameters, and maybe some nodes
        node %= class_name >> -object_name >> '{' >> *parameter >> *node > '}';
        // Rule names
        RULE_NAME(name);
        RULE_NAME(joker);
        RULE_NAME(class_name);
        RULE_NAME(object_name);
        RULE_NAME(unconditional_value);
        RULE_NAME(value);
        RULE_NAME(quoted_value);
        RULE_NAME(unquoted_value);
        RULE_NAME(unquoted_value_item);
        RULE_NAME(trailing_list);
        RULE_NAME(list_value);
        RULE_NAME(conditional_value);
        RULE_NAME(transform);
        RULE_NAME(attributed_name);
        RULE_NAME(parameter);
        node.name("box");
        // Error handling
        qi::on_error<qi::fail>
        (
            node,
            (std::ostream&) _error
                << val("Error! Expecting ")
                << qi::_4                              // what failed?
                << val(" here: \"")
                << construct<std::string>(qi::_3, qi::_2)   // iterators to error-pos, end
                << val("\"")
                << std::endl
        );
    }
};

#pragma clang diagnostic pop

bool parse_boxes(Iterator begin, Iterator end, Node &result);

}

#endif
