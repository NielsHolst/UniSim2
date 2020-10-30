#ifndef COMMAND_DOCUMENTATION_H
#define COMMAND_DOCUMENTATION_H
#include <base/documentation.h>
#include <base/publish.h>

namespace command {

class commandDocumentation : public base::Documentation
{
public:
    commandDocumentation(QString name, QObject *parent);
};

} //namespace

#endif
