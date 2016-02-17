#ifndef AST_H
#define AST_H
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/optional.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/variant.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <QString>

namespace fusion = boost::fusion;
namespace phoenix = boost::phoenix;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
using qi::lexeme;
using ascii::char_;

namespace ast {

typedef boost::optional<std::string> OptionalValue;

struct NameValuePair {
    std::string name, value;
    QString toString() const;
};

struct ParameterWithAttributes {
    std::string name;
    std::vector<NameValuePair> attributes;
    QString toString() const;
};

struct Parameter {
    ParameterWithAttributes attributedName;
    OptionalValue value;
    QString toString() const;
};

struct Node;
typedef boost::recursive_wrapper<Node> CompositeNode;

struct Node {
    std::string className, objectName;
    std::vector<Parameter> parameters;
    std::vector<CompositeNode> children;
    QString toString() const;
    void clear();
private:
    void addToString(QString &s, int level) const;
};

}

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

typedef std::string::const_iterator Iterator;

bool parse(Iterator begin, Iterator end, Node &result);



} //ast


#endif
