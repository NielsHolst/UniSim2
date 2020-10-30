/* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "boxes_documentation.h"

using namespace base;

namespace boxes {

PUBLISH(boxesDocumentation)

boxesDocumentation::boxesDocumentation(QString name, QObject *parent)
    : Documentation(name, parent)
{
    title = "Basic building blocks";
    authorNames
            << "Niels Holst";
    authorInstitutes
            << "Aarhus University";
    authorEmails
            << "niels.holst@agro.au.dk";
}

} //namespace

