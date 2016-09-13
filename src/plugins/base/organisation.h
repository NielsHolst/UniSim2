#ifndef BASE_ORGANISATION_H
#define BASE_ORGANISATION_H

#include <QApplication>
#include "version.h"

namespace base {

struct Organisation{
    Organisation() {
        QCoreApplication::setOrganizationName("AarhusUniversity");
        QCoreApplication::setOrganizationDomain("www.ecolmod.org");
        QCoreApplication::setApplicationName("Universal Simulator");
        QCoreApplication::setApplicationVersion(version());
    }
};

Organisation organisation;

}

#endif // ORGANISATION_H

