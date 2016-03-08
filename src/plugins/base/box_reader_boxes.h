#ifndef BOX_READER_BOXES_H
#define BOX_READER_BOXES_H

#include <fstream>
#include "box_reader_base.h"

namespace ast {
    class Node;
}
namespace base {

class BoxReaderBoxes : public BoxReaderBase
{
public:
    BoxReaderBoxes(BoxBuilder *builder);
    void parse(QString filePath);
private:
    // Data
    std::ifstream _file;
    // Methods
    void openFile(QString filePath);
    bool parse(ast::Node &astRoot);
};

}

#endif
