#ifndef BOX_READER_XML_H
#define BOX_READER_XML_H

#include <QFile>
#include <QMap>
#include <QSet>
#include <QXmlStreamReader>
#include "box_reader_base.h"

namespace base {

class BoxReaderXml : public BoxReaderBase
{
public:
    BoxReaderXml(BoxBuilder *builder);
    void parse(QString filePath);
private:
    // Data
    QXmlStreamReader _reader;
    QFile _file;
    enum ElementType {BoxElement, PortElement};
    ElementType _elementType;
    // Methods
    void openReader(QString filePath);
    void setElementType();
    void setBoxAttributes();
    void setPortAttributes();
};

}

#endif
