#include <QXmlStreamAttributes>
#include "box.h"
#include "box_reader_xml.h"
#include "dialog.h"
#include "exception.h"

namespace base {

BoxReaderXml::BoxReaderXml()
{
    validAttributes[BoxElement]
            << "class" << "name";
    validAttributes[PortElement]
            << "name" << "value" << "ref" << "label" << "axis" << "track" << "page" << "group";
}

BoxBuilder BoxReaderXml::parse(QString filePath) {
    openReader(filePath);
    _reader.readNext();
    if (_reader.tokenType() != QXmlStreamReader::StartDocument)
        throw Exception("XML syntax error at start of document");
    _reader.readNext();
    while (!_reader.atEnd()) {
        dialog().information(_reader.tokenString()+":");
        switch (_reader.tokenType()) {
            case QXmlStreamReader::StartElement:
            setElementType();
            setAttributes();
            switch (_elementType) {
                case BoxElement:
                    _builder.box(_attr.className);
                    _builder.name(_attr.name);
                    dialog().information("box "+_attr.className+" "+_attr.name);
                    break;
                case PortElement:
                    setPortName();
                    setValue();
                    setRef();
                    setLabel();
                    setAxis();
                    setTrackOnOff();
                    _builder.page(_attr.page);
                    _builder.group(_attr.group);
                    break;
            }
            break;
            case QXmlStreamReader::EndElement:
                setElementType();
                switch (_elementType) {
                    case BoxElement:
                        _builder.endbox();
                        dialog().information("endbox");
                        break;
                    case PortElement:
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
    else if (name == "port")
        _elementType = PortElement;
    else
        throw Exception("Unknown XML element. " + currentInfo(), name);
}

void BoxReaderXml::setAttributes() {
    checkAttributes();
    QXmlStreamAttributes attr = _reader.attributes();
    _attr.className = attr.value("class").toString();
    _attr.name = attr.value("name").toString();
    _attr.value = attr.value("value").toString();
    _attr.ref = attr.value("ref").toString();
    _attr.label = attr.value("label").toString();
    _attr.axis = attr.value("axis").toString();
    _attr.track = attr.value("track").toString();
    _attr.page = blankAsNa(attr.value("page").toString());
    _attr.group = blankAsNa(attr.value("group").toString());
}

void BoxReaderXml::checkAttributes() {
    QSet<QString> attributes;
    for (QXmlStreamAttribute attr : _reader.attributes()) {
        QString name = attr.name().toString();
        if (!name.isEmpty())
            attributes << name;
    }

    QSet<QString> difference = attributes.subtract(validAttributes.value(_elementType));

    if (!difference.isEmpty()) {
        QStringList invalidAttr = QStringList(difference.toList());
        QString msg = "Invalid attribute(s). "  + currentInfo();
        QString value = invalidAttr.join(" ");
        throw Exception(msg, value);
    }
}

void BoxReaderXml::setPortName() {
    if (_attr.name.isEmpty())
        throw Exception("Missing port name. " + currentInfo());
    _builder.port(_attr.name);
}

void BoxReaderXml::setValue() {
    if (_reader.attributes().hasAttribute("value"))
        _builder.equals(_attr.value);
}

void BoxReaderXml::setRef() {
    if (_reader.attributes().hasAttribute("ref"))
        _builder.imports(_attr.ref);
}

void BoxReaderXml::setLabel() {
    if (_reader.attributes().hasAttribute("label"))
        _builder.label(_attr.label);
}

void BoxReaderXml::setAxis() {
    if (_reader.attributes().hasAttribute("axis"))
        _builder.axis(_attr.axis);
}

void BoxReaderXml::setTrackOnOff() {
    if (!_attr.track.isEmpty()) {
        if (_attr.track == "on")
            _builder.trackOn();
        else if (_attr.track == "off")
            _builder.trackOff();
        else
            throw Exception("Track value must be 'on' or 'off'. " + currentInfo(), _attr.track);
    }
}

QString BoxReaderXml::currentInfo() const {
    QString info{"\n  Last box: '%1'\n  Last port: '%2'"};
    QString lastBox = _builder.currentBox() ? _builder.currentBox()->objectName() : QString();
    QString lastPort = _builder.currentPort() ? _builder.currentPort()->objectName() : QString();
    return info.arg(lastBox).arg(lastPort);
}

QString BoxReaderXml::blankAsNa(QString s) {
    return (s.isEmpty()) ? QString("NA") : s;
}

} // namespace
