#ifndef ORGANISATION_H
#define ORGANISATION_H

#include <QApplication>

namespace base {

struct Organisation{
    Organisation() {
        QCoreApplication::setOrganizationName("Aarhus University");
        QCoreApplication::setOrganizationDomain("www.ecolmod.org");
        QCoreApplication::setApplicationName("Universal Simulator");
    }
};

Organisation organisation;

}

#endif // ORGANISATION_H

