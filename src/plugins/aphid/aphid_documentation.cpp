#include "aphid_documentation.h"

using namespace base;

namespace aphid {

PUBLISH(aphidDocumentation)

aphidDocumentation::aphidDocumentation(QString name, QObject *parent)
    : Documentation(name, parent)
{
    title = "A model of wheat-aphids-entomopathogenic fungi population dynamics";
    authorNames
            << "Stéphanie Saussure"
            << "Niels Holst";
    authorInstitutes
            << "Norwegian Institute of Bioeconomy Research"
            << "Aarhus University";
    authorEmails
            << "stephanie.saussure@gmail.com"
            << "niels.holst@agro.au.dk";
}

} //namespace

