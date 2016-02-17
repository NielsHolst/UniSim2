#include "ast_boxes.h"

namespace ast {

bool parse_boxes(Iterator begin, Iterator end, Node &result) {
    node_parser<Iterator> parser;
    using boost::spirit::ascii::space;
    bool ok = phrase_parse(begin, end, parser, space, result);
    return (ok && begin == end);
}

} // ast
