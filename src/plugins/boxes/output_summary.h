/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef OUTPUT_SUMMARY_H
#define OUTPUT_SUMMARY_H
#include <QSet>
#include <QString>
#include <QVector>

namespace base {
  class Box;
}

namespace boxes {

class OutputSummary
{
public:
    OutputSummary();
    OutputSummary(QVector<QString> summaries, base::Box *parent);
    enum class Type{Current, Sum, Mean, Min, Max, MinAt, MaxAt};
    bool contains(Type summary) const;
    bool hasNone() const;
    int size() const;
private:
    // Data
    QSet<Type> _summaries;
    base::Box *_parent;
};

inline bool OutputSummary::contains(Type summary) const {
    return _summaries.contains(summary);
}

inline bool OutputSummary::hasNone() const {
    return _summaries.isEmpty();
}

inline int OutputSummary::size() const {
    return _summaries.size();
}

inline uint qHash(const OutputSummary::Type &type) {
    return static_cast<uint>(type);
}

} // boxes

#endif
