#include "cmbp_documentation.h"

using namespace base;

namespace cmbp {

PUBLISH(cmbpDocumentation)

cmbpDocumentation::cmbpDocumentation(QString name, QObject *parent)
    : Documentation(name, parent)
{
    title = "Models of grain store pest population dynamics";
    authorNames
            << "Niels Holst"
            << "William Meikle";
    authorInstitutes
            << "Aarhus University"
            << "USDA-ARS, Tucson, AZ";
    authorEmails
            << "niels.holst@agro.au.dk"
            << "william.meikle@usda.gov";
}

} //namespace

