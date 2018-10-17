#include <string>
#include <QString>
#include "ast_boxes.h"
#include "box.h"
#include "box_reader_boxes.h"
#include "dialog.h"
#include "exception.h"

using namespace std;

namespace base {

BoxReaderBoxes::BoxReaderBoxes(BoxBuilder *builder)
    : BoxReaderBase(builder)
{
}

void BoxReaderBoxes::parse(QString filePath) {
    openFile(filePath);
    ast::Node astRoot;
    if (parse(astRoot))
        astRoot.addToBuilder(*_builder);
    else
        dialog().error("Parse failure");
}

void BoxReaderBoxes::openFile(QString filePath) {
    _file.setFileName(filePath);
    if (!_file.open(QIODevice::ReadOnly|QIODevice::Text)) {
        QString msg("Could not open file for reading");
        ThrowException(msg).value(filePath);
    }
}

bool BoxReaderBoxes::parse(ast::Node &astRoot) {
    std::string storage = QString(_file.readAll()).toStdString();
    // Make certain file ends with a line break
    storage += "\n";
    return ast::parse_boxes(storage.begin(), storage.end(), astRoot);
}

} // namespace
