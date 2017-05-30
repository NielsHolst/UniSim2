#ifndef LIST_H
#define LIST_H
#include <base/command.h>
#include "list_options.h"

namespace command {

class list : public base::Command
{
public:
    list(QString name, QObject *parent);

private:
    // Data
    ListOptionSet _options;
    // Methods
    void doExecute();
};

}

#endif
