/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_BOX_READER_BOXES_H
#define BASE_BOX_READER_BOXES_H

#include "box_reader_base.h"

namespace ast {
    struct Node;
}
namespace base {

class BoxReaderBoxes : public BoxReaderBase
{
public:
    BoxReaderBoxes(BoxBuilder *builder);
    void parse(QString filePath);
private:
    // Data
    QString _filePath;
    // Methods
    bool parse(ast::Node &astRoot);
};

}

#endif
