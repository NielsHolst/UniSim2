#include <QXmlStreamAttributes>
#include "box.h"
#include "box_reader_xml.h"
//#include "dialog.h"
#include "exception.h"

namespace base {

BoxReaderXml::BoxReaderXml(BoxBuilder *builder)
    : BoxReaderBase(builder)
{
}

void BoxReaderXml::parse(QString filePath) {
    openReader(filePath);
    _reader.readNext();
    if (_reader.tokenType() != QXmlStreamReader::StartDocument)
        throw Exception("XML syntax error at start of document");
    _reader.readNext();
    while (!_reader.atEnd()) {
        switch (_reader.tokenType()) {
            case QXmlStreamReader::StartElement:
            setElementType();
            switch (_elementType) {
                case BoxElement:
                    setBoxAttributes();
                    break;
                case PortElement:
                    setPortAttributes();
                    break;
            }
            break;
            case QXmlStreamReader::EndElement:
                setElementType();
                switch (_elementType) {
                    case BoxElement:
                        _builder->endbox();
                        break;
                    case PortElement:
                        break;
                }
                break;
            default:
                break;
        }
        _reader.readNext();
    }
}

void BoxReaderXml::openReader(QString filePath) {
    _file.setFileName(filePath);
    if (!_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString msg("Could not open file '%1' for reading");
        throw Exception(msg.arg(filePath));
    }
    _reader.setDevice(&_file);
}

void BoxReaderXml::setElementType() {
    QString name = _reader.name().toString();
    if (name == "box")
        _elementType = BoxElement;
    else if (name == "port")
        _elementType = PortElement;
    else
        throw Exception("Unknown XML element. " + currentInfo(), name);
}

void BoxReaderXml::setBoxAttributes() {
    // If box has no "class" attribute then default to "box" class
    if (!_reader.attributes().hasAttribute("class"))
        _builder->box("box");
    // Otherwise the "class" attribute will be picked up in the loop
    for (QXmlStreamAttribute attribute : _reader.attributes()) {
        QString name = attribute.name().toString(),
                value = _reader.attributes().value(name).toString();
        if (name == "class")
            _builder->box(value);
        else if (name == "name")
            _builder->name(value);
        else
            throw Exception("Unexpected class attribute" + currentInfo(), name);
    }
}

void BoxReaderXml::setPortAttributes() {
    bool nameSet{false};
    for (QXmlStreamAttribute attribute : _reader.attributes()) {
        QString name = attribute.name().toString(),
                value = _reader.attributes().value(name).toString();
        if (name == "name"){
            _builder->port(value);
            nameSet = true;
        }
        else if (name == "value")
            _builder->equals(value);
        else if (name == "ref")
            _builder->imports(value);
        else
            _builder->attribute(name, value);
    }
    if (!nameSet)
        throw Exception("Port misses \"name\" attribute" + currentInfo());
}

} // namespace
