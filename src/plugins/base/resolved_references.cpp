/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <iostream>
#include <QHash>
#include "computation.h"
#include "exception.h"
#include "node.h"
#include "phys_math.h"
#include "port.h"
#include "resolved_references.h"

namespace base { namespace ResolvedReferences {

// Data
QHash<ResolvedReference, Computation::Step> _table;
int _sizeAtLatestCheck;
bool _isFixed;

// Functions

void clear() {
    _table.clear();
    _sizeAtLatestCheck = 0;
    _isFixed = false;
}

void add(const Node *referee, QVector<Port *> ports) {
    if (Computation::currentStep() <= Computation::Step::Amend) {
        if (!_table.isEmpty())
            ThrowException("Table of resolved references not cleared");
        return;
    }
    dump("addResolvedReferences A");
    for (const Port *port : ports) {
        if (!port->value().isNull()) {
            auto ref = ResolvedReference {referee, port};
            _table[ref] = Computation::currentStep();
        }
    }
    dump("addResolvedReferences Z");
}

void check() {
    if (!_isFixed) {
        if (_table.size() == _sizeAtLatestCheck)
            _isFixed = true;
        else
            _sizeAtLatestCheck = _table.size();
    }
}

bool fixed() {
    return _isFixed;
}

QStringList dump(QString text) {
    std::cout << qPrintable(text) << " dump: " << _table.size() << std::endl;
    QStringList list;
    auto it = _table.constBegin();
    while (it != _table.constEnd()) {
        const Node *referee   = it.key().referee;
        const Port *reference = it.key().reference;
        QString step = Computation::toString(it.value());
         list << (referee->fullName() + " => " + reference->fullName() + " -- " + step);
        ++it;
    }
    std::cout << qPrintable(list.join("\n")) << std::endl;
    return list;
}

size_t qHash(const ResolvedReference &key) {
    return phys_math::hashPointers(key.referee, key.reference);
}

bool operator==(const ResolvedReference &a, const ResolvedReference &b) {
    return a.referee==b.referee && a.reference==b.reference;
}


}}
