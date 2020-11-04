#include "PestTox_documentation.h"

using namespace base;

namespace PestTox {

PUBLISH(PestToxDocumentation)

PestToxDocumentation::PestToxDocumentation(QString name, QObject *parent)
    : Documentation(name, parent)
{
    title = "A pesticide eco-toxicological model";
    authorNames
            << "Mwema Felix"
            << "Niels Holst";
    authorInstitutes
            << "Tropical Pesticides Research Institute, Arusha"
            << "Aarhus University";
    authorEmails
            << "mwema.mwema@tpri.go.tz"
            << "niels.holst@agro.au.dk";
}

} //namespace

