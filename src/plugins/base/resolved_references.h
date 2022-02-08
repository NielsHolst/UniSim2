/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_RESOLVED_REFERENCES_H
#define BASE_RESOLVED_REFERENCES_H
#include <QStringList>
#include <QVector>

namespace base {

class Node;
class Port;

namespace ResolvedReferences {

struct ResolvedReference {
    const Node *referee;
    const Port *reference;
};

void clear();
void add(const Node *referee, QVector<Port *> ports);
void check();
bool fixed();
QStringList dump(QString text);

size_t qHash(const ResolvedReference &key);
bool operator==(const ResolvedReference &a, const ResolvedReference &b);

}}

#endif
