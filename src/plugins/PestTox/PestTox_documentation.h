#ifndef PESTTOX_DOCUMENTATION_H
#define PESTTOX_DOCUMENTATION_H
#include <base/documentation.h>
#include <base/publish.h>

namespace PestTox {

class PestToxDocumentation : public base::Documentation
{
public:
    PestToxDocumentation(QString name, QObject *parent);
};

} //namespace

#endif
