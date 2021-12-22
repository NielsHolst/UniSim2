/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_BOX_BUILDER_H
#define BASE_BOX_BUILDER_H
#include <QList>
#include <QObject>
#include <QStack>
#include <QString>
#include "caller.h"
#include "exception.h"
#include "port.h"

namespace base {

class Box;

class BoxBuilder
{
public:
    BoxBuilder(Box *parent=nullptr);
    ~BoxBuilder();
    void clear();
    // Box
    BoxBuilder& box(Box *box);
    BoxBuilder& box(QString className="Box");
    BoxBuilder& moveToBox(Box *box);
    BoxBuilder& name(QString boxName);
    BoxBuilder& endbox();
    // Port
    BoxBuilder& port(QString name);
    BoxBuilder& aux(QString name);
    BoxBuilder& imports(QString pathToPort, Caller caller=Caller());
    // Set value
    template <class T> BoxBuilder& equals(T value, bool ignore=false);
    BoxBuilder& equals(const char *value, bool ignore=false);
    // State
    const Box* currentBox() const;
    const Port* currentPort() const;
    enum class Amend {Family, Descendants, None} ;
    Box* content(Amend amendOption=Amend::Family, bool allowException=true);
private:
    // Data
    bool _hasParent;
    Box *_content, *_currentBox;
    Port *_currentPort;
    int _exceptionCount;
    QStack<Box*> _stack;
};

template <class T> BoxBuilder& BoxBuilder::equals(T value, bool ignore) {
    if (!ignore && !_currentPort)
        ThrowException("BoxBuilder: 'equals' must follow 'port'");
    _currentPort->equals(value);
    return *this;
}

}
#endif
