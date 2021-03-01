/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_BOX_READER_XML_H
#define BASE_BOX_READER_XML_H

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
    enum ElementType {BoxElement, PortElement, NewPortElement};
    ElementType _elementType;
    // Methods
    typedef BoxBuilder& (BoxBuilder::*BuilderMethod) (QString name);
    void openReader(QString filePath);
    void setElementType();
    void setBoxAttributes();
    void setPortAttributes(BuilderMethod portMethod);
};

}

#endif
