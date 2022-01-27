/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_COMPUTATIONSTEP_H
#define BASE_COMPUTATIONSTEP_H
#include <QString>

namespace base {

class Node;

class Computation {
public:
    enum class Step {Ready, Construct, Amend, Initialize, Reset, Update, Cleanup, Debrief};
    static Step lookup(QString step, Node *context=nullptr);
    static QString toString(Step step);
    static void changeStep(Step step);
    static Step currentStep();
private:
    static Step _currentStep;
    static int _numResolvedReferences;
    static void checkResolvedReferences();
};

inline bool operator<(Computation::Step a, Computation::Step b)
    { return static_cast<int>(a) < static_cast<int>(b); }

inline bool operator<=(Computation::Step a, Computation::Step b)
    { return static_cast<int>(a) <= static_cast<int>(b); }

inline bool operator>(Computation::Step a, Computation::Step b)
    { return static_cast<int>(a) > static_cast<int>(b); }

inline bool operator>=(Computation::Step a, Computation::Step b)
    { return static_cast<int>(a) >= static_cast<int>(b); }

}

#endif
