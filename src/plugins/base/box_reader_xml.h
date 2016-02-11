#ifndef BOX_READER_XML_H
#define BOX_READER_XML_H

#include <QFile>
#include <QMap>
#include <QSet>
#include <QXmlStreamReader>
#include "box_builder.h"

namespace base {

class BoxReaderXml
{
public:
    BoxReaderXml();
    BoxBuilder parse(QString filePath);
private:
    // Data
    QXmlStreamReader _reader;
    BoxBuilder _builder;
    QFile _file;
    enum ElementType {BoxElement, PortElement};
    ElementType _elementType;
    struct {
        QString className, name, value, ref, label, axis, track, page, group;
    } _attr;
    QMap<ElementType, QSet<QString>> validAttributes;
    // Methods
    void openReader(QString filePath);
    void setElementType();
    void setAttributes();
    void checkAttributes();
    void setPortName();
    void setValue();
    void setRef();
    void setLabel();
    void setAxis();
    void setTrackOnOff();
    QString currentInfo() const;
    QString blankAsNa(QString s);

};

}

#endif
