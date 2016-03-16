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
        ThrowException("XML syntax error at start of document");
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
        QString msg("Could not open file for reading");
        ThrowException(msg).value(filePath);
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
        ThrowException("Unknown XML element. " + currentInfo()).value(name);
}

void BoxReaderXml::setBoxAttributes() {
    // If box has no "class" attribute then default to "box" class
    QString className = _reader.attributes().hasAttribute("class") ?
                        _reader.attributes().value("class").toString() :
                        QString("Box");
    _builder->box(className);
    // Box name is optional
    if (_reader.attributes().hasAttribute("name"))
        _builder->name( _reader.attributes().value("name").toString() );
    // Check for unknown attributes
    for (QXmlStreamAttribute attribute : _reader.attributes()) {
        QString name = attribute.name().toString();
        if (name != "class" && name != "name")
            ThrowException("Unexpected class attribute" + currentInfo()).value(name);
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
        ThrowException("Port misses \"name\" attribute" + currentInfo());
}

} // namespace
