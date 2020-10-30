#ifndef BOXES_DOCUMENTATION_H
#define BOXES_DOCUMENTATION_H
#include <base/documentation.h>
#include <base/publish.h>

namespace boxes {

class boxesDocumentation : public base::Documentation
{
public:
    boxesDocumentation(QString name, QObject *parent);
};

} //namespace

#endif
