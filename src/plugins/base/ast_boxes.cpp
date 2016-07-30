#include "ast_boxes.h"
#include "dialog.h"


namespace ast {

bool parse_boxes(Iterator begin, Iterator end, Node &result) {
    node_grammar<Iterator> parser;
    comment_skipper<Iterator> skipper;

    bool ok = phrase_parse(begin,
                           end,
                           parser,
                           skipper,
                           result);

    std::string errorText = parser._error.str();
    if (!ok && !errorText.empty()) {
        QString msg = QString::fromStdString(errorText);
        base::dialog().error(msg.replace("\t", "  "));
    }
    return (ok && begin == end);
}

} // ast
