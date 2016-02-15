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

namespace grammar {

    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;

    struct name_value_pair {
        std::string name, value;
    };

    struct node {
        std::string className, objectName;
        std::vector<name_value_pair> parameters;
    };
}

BOOST_FUSION_ADAPT_STRUCT(
    grammar::node,
    (std::string, className)
    (std::string, objectName)
    (std::vector<grammar::name_value_pair>, parameters)
)

BOOST_FUSION_ADAPT_STRUCT(
    grammar::name_value_pair,
    (std::string, name)
    (std::string, value)
)

namespace grammar {
    template <typename Iterator>
    struct node_parser : qi::grammar<Iterator, ascii::space_type, node()>
    {
        qi::rule<Iterator, ascii::space_type, node()> start;
        qi::rule<Iterator, ascii::space_type, std::string()> class_name, object_name, identifier, value;
        qi::rule<Iterator, ascii::space_type, name_value_pair()> parameter;
        qi::rule<Iterator, ascii::space_type, std::vector<name_value_pair>()> parameters;

        node_parser() : node_parser::base_type(start) {
            using qi::lexeme;
            using ascii::char_;

            start %= class_name >> -object_name >> parameters  >> '{' >> '}';
            class_name %= identifier;
            object_name %= identifier;
            identifier %= lexeme[char_("a-zA-Z_") >> *char_("a-zA-Z0-9_")];
            value %= lexeme[+(char_ - char_(" )"))];
            parameter %= identifier >> '=' >> value;
            parameters %= '(' >> *parameter >> ')';
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
    grammar::node ast;

    bool ok = phrase_parse(iter, end, parser, space, ast);
    if (ok && iter == end) {
        std::string s = "Parse success\n  ";
        s += ast.className + " ";
        s += ast.objectName + ": ";
        if (ast.parameters.size() == 0)
            s+= "No parameters";
        else {
            for (auto it=ast.parameters.begin(); it!=ast.parameters.end(); ++it)
                s += it->name + " equals '" + it->value + "' ";
        }
        dialog().information(QString::fromStdString(s));
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
