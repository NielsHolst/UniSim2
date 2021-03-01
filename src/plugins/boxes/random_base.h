/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef RANDOM_BASE_H
#define RANDOM_BASE_H
#include <base/box.h>

namespace base {
    class RandomOrder;
}

namespace boxes {

class RandomiserBase;

class RandomBase : public base::Box
{
public: 
    RandomBase(QString name, QObject *parent);
    virtual ~RandomBase() {}
    void amend() final;
    void initialize() final;
    void reset() final;
    void update() final;
    void import() final;
    RandomiserBase* randomiser();
protected:
    // Inputs
    double P;
    bool
        useFixed,
        drawAtInitialize,
        drawAtReset,
        drawAtUpdate;
    // Data
    base::RandomOrder *_order;
private:
    RandomiserBase *_randomiser;
    bool _setNumStrata;
    // Methods
    virtual void checkInputs() = 0;
    void setNumStrata();
protected:
    virtual void updateValue() = 0;
};

} //namespace
#endif
