#ifndef BASE_ORGANISATION_H
#define BASE_ORGANISATION_H

#include <QApplication>

namespace base {

struct Organisation{
    Organisation() {
        QCoreApplication::setOrganizationName("AarhusUniversity");
        QCoreApplication::setOrganizationDomain("www.ecolmod.org");
        QCoreApplication::setApplicationName("Universal Simulator");
    }
};

Organisation organisation;

}

#endif // ORGANISATION_H

