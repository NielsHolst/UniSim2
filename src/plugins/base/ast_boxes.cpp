#include "ast_boxes.h"
#include "dialog.h"

namespace ast {

bool parse_boxes(Iterator begin, Iterator end, Node &result) {
    node_parser<Iterator> parser;
    using boost::spirit::ascii::space;
    bool ok = phrase_parse(begin, end, parser, space, result);
    std::string errorText = parser._error.str();
    if (!ok && !errorText.empty()) {
        QString msg = QString::fromStdString(errorText);
        base::dialog().information(msg.replace("\t", "  "));
    }
    return (ok && begin == end);
}

} // ast
