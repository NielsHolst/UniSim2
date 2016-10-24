/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#include "field_area.h"
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH (fieldarea)
	
fieldarea::fieldarea(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(W).equals(110.);     //width of the field (m)
    Input(L).equals(90.);      //length of the field (m)
    Output(area);
}

void fieldarea::reset() {
    area = 0.;
}

void fieldarea::update() {
    area = W * L;
}

} //namespace

