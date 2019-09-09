/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "ast_boxes.h"
#include "dialog.h"

// Note: ast_common processes the structs generated here

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
