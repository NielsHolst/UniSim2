/* Copyright (C) 2009-2016 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef PRIMARY_DISTRIBUTION_BASE_H
#define PRIMARY_DISTRIBUTION_BASE_H

#include <base/box.h>

namespace PestTox {

class PrimaryDistributionBase : public base::Box
{
public:
    PrimaryDistributionBase(QString name, QObject *parent);
protected:
    // Input
    double fieldArea, doseApplied;
    // Output
    double doseTaken, fractionTaken;
};

} //namespace


#endif
