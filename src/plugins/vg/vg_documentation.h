#ifndef VG_DOCUMENTATION_H
#define VG_DOCUMENTATION_H
#include <base/documentation.h>
#include <base/publish.h>

namespace vg {

class vgDocumentation : public base::Documentation
{
public:
    vgDocumentation(QString name, QObject *parent);
};

} //namespace

#endif
