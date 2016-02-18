#ifndef BOX_READER_BASE_H
#define BOX_READER_BASE_H

#include "box_builder.h"

namespace base {

class BoxReaderBase
{
public:
    BoxReaderBase();
    virtual ~BoxReaderBase(){}
    virtual BoxBuilder parse(QString filePath) = 0;
protected:
    // Data
    BoxBuilder _builder;
    // Methods
    QString currentInfo() const;
};

}

#endif
