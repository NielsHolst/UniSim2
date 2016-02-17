#ifndef AST_BOXES_H
#define AST_BOXES_H
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/variant.hpp>
#include "ast_common.h"

namespace fusion = boost::fusion;
namespace phoenix = boost::phoenix;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
using qi::lexeme;
using ascii::char_;

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
    (std::string, name)
    (std::vector<ast::NameValuePair>, attributes)
)

BOOST_FUSION_ADAPT_STRUCT(
    ast::Parameter,
    (ast::ParameterWithAttributes, attributedName)
    (ast::OptionalValue, value)
)

namespace ast {

template <typename Iterator>
struct node_parser : qi::grammar<Iterator, ascii::space_type, Node()>
{
    qi::rule<Iterator, ascii::space_type, Node()> node;
    qi::rule<Iterator, ascii::space_type, std::string()>
            class_name, object_name, name, value, unquoted_value, quoted_value;
    qi::rule<Iterator, ascii::space_type, OptionalValue()> optional_value;
    qi::rule<Iterator, ascii::space_type, NameValuePair()> name_value_pair;
    qi::rule<Iterator, ascii::space_type, std::vector<NameValuePair>()> attributes;
    qi::rule<Iterator, ascii::space_type, ParameterWithAttributes()> attributed_name;
    qi::rule<Iterator, ascii::space_type, Parameter()> parameter;
    qi::rule<Iterator, ascii::space_type, std::vector<Parameter>()> parameters;
    qi::rule<Iterator, ascii::space_type, std::vector<CompositeNode>()> body;

    node_parser() : node_parser::base_type(node) {
        // A name has C++ identifier style
        name %= lexeme[char_("a-zA-Z_") >> *char_("a-zA-Z0-9_")];
        // Classes and objects have a name
        class_name %= name;
        object_name %= name;
        // A value may be quotes or unquoted
        value = (quoted_value | unquoted_value);
        // A quoted value is a string of any characters, except apostrophes;
        // bracing apostrophes are kept in the string
        quoted_value %= lexeme[char_('"') >> *(char_ - '"') >> char_('"')];
        // An unquoted value is for numbers, dates and times (booleans??)
        unquoted_value %= lexeme[+(char_ - char_(" \"(){}"))];
        // A value may be optional
        optional_value %= value;
        // A name-value pair
        name_value_pair %= name >> '=' >> value;
        // Attributes as a list of name-value pairs
        attributes %= '{' >> *name_value_pair >> '}';
        // A name with optional attributes
        attributed_name %= name >> -attributes;
        // A parameter has a name, maybe with attributes, and maybe with a value
        parameter %= attributed_name >> -('=' >> optional_value);
        // A list of parameters
        parameters %= '(' >> *parameter >> ')';
        // A body with some nodes
        body %= '{' >> *node >> '}';
        // A node has a class name, maybe an object name, maybe parameters, and maybe a body
        node %= class_name >> -object_name >> -parameters  >> -body;
    }
};

bool parse_boxes(Iterator begin, Iterator end, Node &result);

}

#endif
