#include <QXmlStreamAttributes>
#include "box.h"
#include "box_reader_xml.h"
#include "exception.h"

namespace base {

BoxReaderXml::BoxReaderXml()
{
}

BoxBuilder BoxReaderXml::parse(QString filePath) {
    openReader(filePath);
    _reader.readNext();
    if (_reader.tokenType() != QXmlStreamReader::StartDocument)
        throw Exception("XML syntax error at start of document");
    _reader.readNext();
    while (!_reader.atEnd()) {
        switch (_reader.tokenType()) {
            case QXmlStreamReader::StartElement:
            setElementType();
            setAttributes();
            switch (_elementType) {
                case BoxElement:
                    _builder.box(_attr.className);
                    _builder.name(_attr.name);
                    break;
                case InputElement:
                    _builder.port(_attr.name).equals(_attr.value);
                    break;
            }
            break;
        case QXmlStreamReader::EndElement:
            switch (_elementType) {
                case BoxElement:
                    _builder.endbox();
                    break;
                case InputElement:
                    break;
            }
            break;
        default:
            break;
        }

//        QString text = _reader.name().toString();
//        text = QString::number((int)_reader.tokenType()) + " " + _reader.tokenString() + text;
//        dialog().information(text);
        _reader.readNext();
    }
    return _builder;
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
    else if (name == "input")
        _elementType = InputElement;
    else
        throw Exception("Unknown XML element", name);
}

void BoxReaderXml::setAttributes() {
    QXmlStreamAttributes attr = _reader.attributes();
    _attr.className = attr.value("class").toString();
    _attr.name = attr.value("name").toString();
    _attr.value = attr.value("value").toString();
}

}

