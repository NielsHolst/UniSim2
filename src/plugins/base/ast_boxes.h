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
namespace ascii = boost::spirit::ascii;
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
    ast::NameValuePair,
    (std::string, name)
    (std::string, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast::ParameterWithAttributes,
    (std::string, type)
    (std::string, name)
    (std::vector<ast::NameValuePair>, attributes)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast::Parameter,
    (ast::ParameterWithAttributes, attributedName)
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

template <typename Iterator, typename Skipper = comment_skipper<Iterator>>
struct node_grammar : public qi::grammar<Iterator, Skipper, Node()>
{
    qi::rule<Iterator, Skipper, Node()> node;
    qi::rule<Iterator, Skipper, std::string()>
            class_name, object_name, name, value, unquoted_value, quoted_value, list_value;
    qi::rule<Iterator, Skipper, NameValuePair()> name_value_pair;
    qi::rule<Iterator, Skipper, std::vector<NameValuePair>()> attributes;
    qi::rule<Iterator, Skipper, ParameterWithAttributes()> attributed_name;
    qi::rule<Iterator, Skipper, Parameter()> parameter;

    std::stringstream _error;

    node_grammar() : node_grammar::base_type(node) {
        // A name has C++ identifier style
        name %= lexeme[char_("a-zA-Z_") >> *char_("a-zA-Z0-9_")];
        // classes and objects have a name;
        // a class name may be qualified by a namespace name
        class_name %= name >> -(char_(':') > char_(':') > name);
        object_name %= name;
        // A value may be quoted or unquoted, or a list of values
        value = (quoted_value | unquoted_value | list_value);
        // A quoted value is a string of any characters, except apostrophes;
        // bracing apostrophes are kept in the string
        quoted_value %= lexeme[char_('"') >> *(char_ - '"') > char_('"')];
        // An unquoted value is for numbers, dates, times and booleans
        unquoted_value %= lexeme[+(char_ - char_(" \t\n \"(){}"))];
        // A list value is captured; keeping parentheses and everything inside
        list_value %= lexeme[char_('(') >> *(char_ - ')') > char_(')')];
        // A name-value pair
        name_value_pair %= name >> '=' > value;
        // Attributes as a list of name-value pairs
        attributes %= '(' >> *name_value_pair > ')';
        // A name with optional attributes
        attributed_name %= char_("\\.+") >> name >> -attributes;
        // A parameter has a name, maybe with attributes, and a value
        parameter %= attributed_name > '=' >> value;
        // A node has a class name, maybe an object name, maybe some parameters, and maybe some nodes
        node %= class_name >> -object_name >> '{' >> *parameter >> *node > '}';
        // Rule names
        RULE_NAME(name);
        RULE_NAME(class_name);
        RULE_NAME(object_name);
        RULE_NAME(value);
        RULE_NAME(quoted_value);
        RULE_NAME(unquoted_value);
        RULE_NAME(list_value);
        RULE_NAME(name_value_pair);
        RULE_NAME(attributes);
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

bool parse_boxes(Iterator begin, Iterator end, Node &result);

}

#endif
