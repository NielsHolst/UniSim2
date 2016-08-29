#ifndef BASE_BOX_READER_BASE_H
#define BASE_BOX_READER_BASE_H

#include "box_builder.h"

namespace base {

class BoxBuilder;

class BoxReaderBase
{
public:
    BoxReaderBase(BoxBuilder *builder);
    virtual ~BoxReaderBase(){}
    virtual void parse(QString filePath) = 0;
protected:
    // Data
    BoxBuilder *_builder;
    // Methods
    QString currentInfo() const;
};

}

#endif
