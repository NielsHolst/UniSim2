#ifndef BOX_READER_XML_H
#define BOX_READER_XML_H

#include <QFile>
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
    enum ElementType {BoxElement, InputElement};
    ElementType _elementType;
    struct {
        QString className, name, value;
    } _attr;
    // Methods
    void openReader(QString filePath);
    void setElementType();
    void setAttributes();
    void readBox();
    void readInput();

};

}

#endif
