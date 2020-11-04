#ifndef MUSSELBED_DOCUMENTATION_H
#define MUSSELBED_DOCUMENTATION_H
#include <base/documentation.h>
#include <base/publish.h>

namespace MusselBed {

class MusselBedDocumentation : public base::Documentation
{
public:
    MusselBedDocumentation(QString name, QObject *parent);
};

} //namespace

#endif
