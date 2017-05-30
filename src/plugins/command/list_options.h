#ifndef LIST_OPTIONS_H
#define LIST_OPTIONS_H
#include <QSet>
#include <base/convert.h>

namespace command {

enum class ListOption{
    Ports,
    Inputs,
    Outputs,
    Imports,
    Exports,
    Boxes,
    Recurse
};

typedef QSet<ListOption> ListOptionSet;

uint qHash(const ListOption &a);

}

namespace base {
    template<> command::ListOption convert(char c);
    template<> command::ListOptionSet convert(QString s);
}

#endif

