#include <string>
#include <vector>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/variant.hpp>
#include <boost/variant/recursive_variant.hpp>
#include "box.h"
#include "box_reader_boxes.h"
#include "dialog.h"
#include "exception.h"

// Spirit grammar

namespace fusion = boost::fusion;
namespace phoenix = boost::phoenix;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
using qi::lexeme;
using ascii::char_;

QString toString(std::vector<std::string> vec) {
    QStringList list;
    for (auto s : vec)
        list << QString::fromStdString(s);
    return list.join("&");
}

namespace grammar {
    struct NameValuePair {
        std::string name, value;
        QString toString() const {
            return QString::fromStdString(name + "~" + value);
        }
    };

    struct ParameterWithAttributes {
        std::string name;
        std::vector<NameValuePair> attributes;
        QString toString() const {
            QString s = QString::fromStdString(name);
            if (attributes.size() > 0) {
                s += " with";
                for (auto attr : attributes)
                    s += " " + attr.toString();
            }
            return s;
        }
    };

    struct Parameter {
        ParameterWithAttributes attributedName;
        std::string value;
        QString toString() const {
            return attributedName.toString() + " equals '" +
                   QString::fromStdString(value) + "'";
        }
    };

    struct Node;
    typedef boost::recursive_wrapper<Node> CompositeNode;
    struct Node {
        std::string className, objectName;
        std::vector<Parameter> parameters;
        std::vector<CompositeNode> children;

        void print(QString &s, int level = 0) const {
            s += QString().fill(' ', 2*level);
            s += QString::fromStdString(className + " " + objectName);
            s += "\n";
            for (auto parameter : parameters) {
                s += QString().fill(' ', 2*(level+1));
                s += parameter.toString() + "\n";
            }
            for (auto child : children) {
                const Node *node = child.get_pointer();
                node->print(s,level+1);
            }
        }
    };
}

BOOST_FUSION_ADAPT_STRUCT(
    grammar::Node,
    (std::string, className)
    (std::string, objectName)
    (std::vector<grammar::Parameter>, parameters)
    (std::vector<grammar::CompositeNode>, children)
)

BOOST_FUSION_ADAPT_STRUCT(
    grammar::NameValuePair,
    (std::string, name)
    (std::string, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    grammar::ParameterWithAttributes,
    (std::string, name)
    (std::vector<grammar::NameValuePair>, attributes)
)

BOOST_FUSION_ADAPT_STRUCT(
    grammar::Parameter,
    (grammar::ParameterWithAttributes, attributedName)
    (std::string, value)
)

namespace grammar {
    template <typename Iterator>
    struct node_parser : qi::grammar<Iterator, ascii::space_type, Node()>
    {
        qi::rule<Iterator, ascii::space_type, Node()> node;
        qi::rule<Iterator, ascii::space_type, std::string()> class_name, object_name, identifier, value;
        qi::rule<Iterator, ascii::space_type, NameValuePair()> nameValuePair;
        qi::rule<Iterator, ascii::space_type, std::vector<NameValuePair>()> attributes;
        qi::rule<Iterator, ascii::space_type, ParameterWithAttributes()> attributedName;
        qi::rule<Iterator, ascii::space_type, Parameter()> parameter;
        qi::rule<Iterator, ascii::space_type, std::vector<Parameter>()> parameters;
        qi::rule<Iterator, ascii::space_type, std::vector<CompositeNode>()> body;

        node_parser() : node_parser::base_type(node) {
            node %= class_name >> -object_name >> -parameters  >> -body;
            class_name %= identifier;
            object_name %= identifier;
            identifier %= lexeme[char_("a-zA-Z_") >> *char_("a-zA-Z0-9_")];
            value %= lexeme[+(char_ - char_(" (){}"))];
            parameters %= '(' >> *parameter >> ')';
            parameter %= attributedName >> -('=' >> value);
            attributedName %= identifier >> -attributes;
            attributes %= '{' >> *nameValuePair >> '}';
            nameValuePair %= identifier >> '=' >> value;
            body %= '{' >> *node >> '}';
        }

    };
}

// BoxReaderBoxes

namespace base {

BoxReaderBoxes::BoxReaderBoxes()
    : BoxReaderBase()
{
}

BoxBuilder BoxReaderBoxes::parse(QString filePath) {
    openFile(filePath);

    std::string storage;
    std::copy(
        std::istream_iterator<char>(_file),
        std::istream_iterator<char>(),
        std::back_inserter(storage));

    typedef std::string::const_iterator Iterator;
    Iterator iter = storage.begin(),
             end = storage.end();
    grammar::node_parser<Iterator> parser;
    using boost::spirit::ascii::space;
    grammar::Node ast;

    bool ok = phrase_parse(iter, end, parser, space, ast);
    if (ok && iter == end) {
        QString s;
        ast.print(s);
        dialog().information(s);
    }
    else
        dialog().information("Parse failure");

    return _builder;
}

void BoxReaderBoxes::openFile(QString filePath) {
    _file.open(filePath.toStdString(), std::ios_base::in);
    if (!_file) {
        QString msg("Could not open file '%1' for reading");
        throw Exception(msg.arg(filePath));
    }
    // No white space skipping
    _file.unsetf(std::ios::skipws);
}

void BoxReaderBoxes::setPortName() {
//    if (_attr.name.isEmpty())
//        throw Exception("Missing port name. " + currentInfo());
//    _builder.port(_attr.name);
}

void BoxReaderBoxes::setValue() {
//    if (_reader.attributes().hasAttribute("value"))
//        _builder.equals(_attr.value);
}

void BoxReaderBoxes::setRef() {
//    if (_reader.attributes().hasAttribute("ref"))
//        _builder.imports(_attr.ref);
}

void BoxReaderBoxes::setLabel() {
//    if (_reader.attributes().hasAttribute("label"))
//        _builder.label(_attr.label);
}

void BoxReaderBoxes::setAxis() {
//    if (_reader.attributes().hasAttribute("axis"))
//        _builder.axis(_attr.axis);
}

void BoxReaderBoxes::setTrackOnOff() {
//    if (!_attr.track.isEmpty()) {
//        if (_attr.track == "on")
//            _builder.trackOn();
//        else if (_attr.track == "off")
//            _builder.trackOff();
//        else
//            throw Exception("Track value must be 'on' or 'off'. " + currentInfo(), _attr.track);
//    }
}


} // namespace
