#include <string>
#include <vector>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/variant.hpp>
#include <boost/variant/recursive_variant.hpp>
#include "box.h"
#include "box_reader_boxes.h"
#include "exception.h"

// Spirit declarations

namespace fusion = boost::fusion;
namespace phoenix = boost::phoenix;
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

struct BoxDef;

typedef boost::variant<boost::recursive_wrapper<BoxDef>> BoxDefNode;

struct BoxDef {
    std::string className, objectName;
    std::vector<BoxDefNode> children;
};

BOOST_FUSION_ADAPT_STRUCT(
    BoxDef,
    (std::string, className)
    (std::string, objectName)
    (std::vector<BoxDefNode>, children)
)

template <typename Iterator>
struct BoxGrammar : qi::grammar<Iterator, BoxDef(), ascii::space_type>
{
    qi::rule<Iterator, BoxDef(), ascii::space_type> box;
    qi::rule<Iterator, BoxDefNode(), ascii::space_type> node;
    qi::rule<Iterator, std::string(), ascii::space_type> className;
    qi::rule<Iterator, std::string(), ascii::space_type> objectName;

    BoxGrammar() : BoxGrammar::base_type(box)
    {
        using qi::lit;
        using qi::lexeme;
        using ascii::char_;
        using ascii::string;
        using namespace qi::labels;
        using phoenix::at_c;
        using phoenix::push_back;

        box = *node;
        node = className>> objectName >> node;
        className = lexeme[+(char_ - '{')        [_val += _1]];
        objectName = lexeme[+(char_ - '{')        [_val += _1]];


    }
};


namespace base {

// BoxReaderBoxes

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
