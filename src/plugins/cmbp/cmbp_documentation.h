#ifndef CMBP_DOCUMENTATION_H
#define CMBP_DOCUMENTATION_H
#include <base/documentation.h>
#include <base/publish.h>

namespace cmbp {

class cmbpDocumentation : public base::Documentation
{
public:
    cmbpDocumentation(QString name, QObject *parent);
};

} //namespace

#endif
