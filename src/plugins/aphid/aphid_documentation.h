#ifndef APHID_DOCUMENTATION_H
#define APHID_DOCUMENTATION_H
#include <base/documentation.h>
#include <base/publish.h>

namespace aphid {

class aphidDocumentation : public base::Documentation
{
public:
    aphidDocumentation(QString name, QObject *parent);
};

} //namespace

#endif
