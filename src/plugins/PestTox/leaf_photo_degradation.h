/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef LEAF_PHOTO_DEGRADATION_H
#define LEAF_PHOTO_DEGRADATION_H

#include "loss_rate.h"

namespace base {
    class DataGrid;
}

namespace PestTox {

class LeafPhotoDegradation : public LossRate
{
public:
    LeafPhotoDegradation(QString name, QObject *parent);
    void initialize();

private:
    // Input
    QString fileName;
    double dayOfYear, latitude, kOH;

    // Output
    double OHconcentration;

    // Data
    base::DataGrid *_freeRadicalsTable;

    // Methods
    double computeInstantaneous();
};

} //namespace


#endif
