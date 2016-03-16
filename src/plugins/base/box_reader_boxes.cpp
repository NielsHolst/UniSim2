#include <string>
#include "ast_boxes.h"
#include "box.h"
#include "box_reader_boxes.h"
#include "dialog.h"
#include "exception.h"

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
    _file.open(filePath.toStdString(), std::ios_base::in);
    if (!_file) {
        QString msg("Could not open file for reading");
        ThrowException(msg).value(filePath);
    }
    // No white space skipping
    _file.unsetf(std::ios::skipws);
}

bool BoxReaderBoxes::parse(ast::Node &astRoot) {
    std::string storage;
    std::copy(
        std::istream_iterator<char>(_file),
        std::istream_iterator<char>(),
        std::back_inserter(storage));
    return ast::parse_boxes(storage.begin(), storage.end(), astRoot);
}

} // namespace
