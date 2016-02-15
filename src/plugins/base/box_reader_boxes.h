#ifndef BOX_READER_BOXES_H
#define BOX_READER_BOXES_H

#include <fstream>
#include "box_reader_base.h"

namespace base {

class BoxReaderBoxes : public BoxReaderBase
{
public:
    BoxReaderBoxes();
    BoxBuilder parse(QString filePath);
private:
    // Data
    std::ifstream _file;
    // Methods
    void openFile(QString filePath);
    void setPortName();
    void setValue();
    void setRef();
    void setLabel();
    void setAxis();
    void setTrackOnOff();
};

}

#endif
