/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef CROP_H
#define CROP_H

#include <QDate>
#include <base/box.h>

namespace PestTox {

class Crop : public base::Box
{
public:
    Crop(QString name, QObject *parent);
    void update();

private:
    // Input
    QDate sowingDate, date;

    // Output
    int sown;
};

} //namespace


#endif
