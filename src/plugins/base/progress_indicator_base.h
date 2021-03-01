/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_PROGRESS_INDICATOR_BASE_H
#define BASE_PROGRESS_INDICATOR_BASE_H

#include <QObject>
#include <QTime>

namespace base {

class ProgressIndicatorBase : public QObject
{
public:
    ProgressIndicatorBase(QObject *parent);
    void reset();
    struct UpdateInfo{
        QTime time;
        int step, steps, iteration, iterations;
    };
    void update(const UpdateInfo &info);
    virtual void show(int current, int total) = 0;
private:
    // Data
    double _nextShowProgress;
};


} //namespace

#endif

