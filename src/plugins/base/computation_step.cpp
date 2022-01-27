/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QMap>
#include "dialog.h"
#include "expression.h"
#include "node.h"
#include "exception.h"
#include "computation_step.h"

namespace base {

Computation::Step Computation::_currentStep = Computation::Step::Ready;
int Computation::_numResolvedReferences = 0;

Computation::Step Computation::lookup(QString step, Node *context) {
    static QMap<QString, Computation::Step> map =
    {
        {"ready"     , Computation::Step::Ready     },
        {"construct" , Computation::Step::Construct },
        {"amend"     , Computation::Step::Amend     },
        {"initialize", Computation::Step::Initialize},
        {"reset"     , Computation::Step::Reset     },
        {"update"    , Computation::Step::Update    },
        {"cleanup"   , Computation::Step::Cleanup   },
        {"debrief"   , Computation::Step::Debrief   },
    };
    if (!map.contains(step)) {
        QStringList list = map.keys();
        QString s = list.join(", ");
        ThrowException("Unknown computation step").value(step).hint(s).context(context);
    }
    return map.value(step);
}

QString Computation::toString(Computation::Step step) {
    static QMap<Computation::Step, QString> map =
    {
        {Computation::Step::Ready     , "ready"     },
        {Computation::Step::Construct , "construct" },
        {Computation::Step::Amend     , "amend"     },
        {Computation::Step::Initialize, "initialize"},
        {Computation::Step::Reset     , "reset"     },
        {Computation::Step::Update    , "update"    },
        {Computation::Step::Cleanup   , "cleanup"   },
        {Computation::Step::Debrief   , "debrief"   }
    };
    return map.value(step);
}

void Computation::changeStep(Step step) {
    static QMap<Computation::Step, QString> map =
    {
        {Computation::Step::Ready     , "Ready..."       },
        {Computation::Step::Construct , "Constructing..."},
        {Computation::Step::Amend     , "Amending..."    },
        {Computation::Step::Initialize, "Initializing..."},
        {Computation::Step::Reset     , "Resetting..."   },
        {Computation::Step::Update    , "Updating..."    },
        {Computation::Step::Cleanup   , "Cleaning up..." },
        {Computation::Step::Debrief   , "Debriefing..."  }
    };

    dialog().message(map.value(step));

    _currentStep = step;

    if (step <= Step::Amend) {
        _numResolvedReferences = 0;
        Expression::resetResolvedReferences();
    }
    else {
        if (Expression::numResolvedReferences() == _numResolvedReferences)
            Expression::fixResolvedReferences();
        else
            _numResolvedReferences = Expression::numResolvedReferences();
    }
}

Computation::Step Computation::currentStep() {
    return _currentStep;
}

}
