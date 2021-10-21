/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef LIST_OUTPUT_H
#define LIST_OUTPUT_H
#include <QVector>
#include "list_options.h"

namespace base {
    class Box;
    class Port;
}

namespace command {

class ListOutput{
public:
    ListOutput(QVector<base::Box *> boxes, ListOptionSet options);
    QString toString();
private:
    // Data
    QVector<base::Box*> _boxes;
    ListOptionSet _options;
    QString _s;
    bool _listInputs, _listOutputs,
         _listImports, _listExports, _listExportsOnly,
         _listShort,
         _recurse;
    // Methods
    void toString(base::Box *box, int level);
    void toString(base::Port *port, int level);
};

}

#endif
