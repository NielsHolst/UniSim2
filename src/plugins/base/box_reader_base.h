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
    virtual void setPortName() = 0;
    virtual void setValue() = 0;
    virtual void setRef() = 0;
    virtual void setLabel() = 0;
    virtual void setAxis() = 0;
    virtual void setTrackOnOff() = 0;
    QString currentInfo() const;
    static QString blankAsNa(QString s);

};

}

#endif
