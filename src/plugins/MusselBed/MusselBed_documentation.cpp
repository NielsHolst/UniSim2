#include "MusselBed_documentation.h"

using namespace base;

namespace MusselBed {

PUBLISH(MusselBedDocumentation)

MusselBedDocumentation::MusselBedDocumentation(QString name, QObject *parent)
    : Documentation(name, parent)
{
    title = "A model of blue mussel-starfish population dynamics";
    authorNames
            << "Niels Holst"
            << "Antonio Aguera Garcia";
    authorInstitutes
            << "Aarhus University"
            << "Institute of Marine Research, Bergen";
    authorEmails
            << "niels.holst@agro.au.dk"
            << "antonio.aguera@hi.no";
}

} //namespace

