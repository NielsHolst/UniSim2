/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <string>
#include "ast_boxes.h"
#include "box_preprocessor.h"
#include "box_reader_boxes.h"
#include "exception.h"

using namespace std;

namespace base {

BoxReaderBoxes::BoxReaderBoxes(BoxBuilder *builder)
    : BoxReaderBase(builder)
{
}

void BoxReaderBoxes::parse(QString filePath) {
    _filePath = filePath;
    ast::Node astRoot;
    if (parse(astRoot))
        astRoot.addToBuilder(*_builder);
    else
        ThrowException("Parse failure");
}

bool BoxReaderBoxes::parse(ast::Node &astRoot) {
    BoxPreprocessor preprocessor;
    QString code = preprocessor.preprocess(_filePath);
    std::string stdCode = code.toStdString();
    return ast::parse_boxes(stdCode.begin(), stdCode.end(), astRoot);
}

} // namespace
