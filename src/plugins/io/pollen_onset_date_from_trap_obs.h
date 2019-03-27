/* Copyright (C) 2009-2012 by Niels Holst [niels.holst@agrsci.dk] and co-authors.
** Copyrights reserved.
** Released under the terms of the GNU General Public License version 3.0 or later.
** See www.gnu.org/copyleft/gpl.html.
*/
#ifndef POLLEN_ONSET_DATE_FROM_TRAP_OBS
#define POLLEN_ONSET_DATE_FROM_TRAP_OBS
#include "pollen_onset_date_base.h"

namespace base {
    class DataGrid;
}

namespace io {

class PollenOnsetDateFromTrapObs : public PollenOnsetDateBase
{
public:
    PollenOnsetDateFromTrapObs(QString name, QObject *parent);
    // standard methods
    void reset();
private:
    // Inputs
    QString fileName;
    // data
    int colDate, colPollen;
    // methods
    int findColumn(const base::DataGrid &data, QString colName);
};


} //namespace
#endif
