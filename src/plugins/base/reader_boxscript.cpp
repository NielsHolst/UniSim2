/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <string>
#include "ast_boxes.h"
#include "boxdcript_preprocessor.h"
#include "reader_boxscript.h"
#include "exception.h"

using namespace std;

namespace base {

ReaderBoxScript::ReaderBoxScript(BoxBuilder *builder)
    : ReaderBase(builder)
{
}

void ReaderBoxScript::parse(QString filePath) {
    _filePath = filePath;
    ast::Node astRoot;
    if (parse(astRoot))
        astRoot.addToBuilder(*_builder);
    else
        ThrowException("Parse failure");
}

bool ReaderBoxScript::parse(ast::Node &astRoot) {
    BoxScriptPreprocessor preprocessor;
    QString code = preprocessor.preprocess(_filePath);
    std::string stdCode = code.toStdString();
    return ast::parse_boxes(stdCode.begin(), stdCode.end(), astRoot);
}

} // namespace
