#include <string>
#include <vector>
#include "ast_boxes.h"
#include "box.h"
#include "box_reader_boxes.h"
#include "dialog.h"
#include "exception.h"

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

    ast::Node ast;
    bool ok = ast::parse_boxes(storage.begin(), storage.end(), ast);
    if (ok)
        dialog().information(ast.toString());
    else
        dialog().information("Parse failure");

    ast.addToBuilder(_builder);

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
