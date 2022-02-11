/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_BOX_BUILDER_H
#define BASE_BOX_BUILDER_H
#include <QList>
#include <QStack>
#include <QString>
#include "caller.h"
#include "exception.h"
#include "expression.h"
#include "port.h"

namespace base {

class Box;

class BoxBuilder
{
public:
    BoxBuilder(Box *parent=nullptr);
    // Box
    BoxBuilder& box(Box *box);
    BoxBuilder& box(QString className="Box");
    BoxBuilder& moveToBox(Box *box);
    BoxBuilder& name(QString boxName);
    BoxBuilder& endbox();
    // Port
    BoxBuilder& port(QString name);
    BoxBuilder& aux(QString name, QString type=QString());
    BoxBuilder& imports(QString pathToPort, Caller caller=Caller());
    // Set value
    template <class T> BoxBuilder& equals(T value);
    BoxBuilder& equals(const char *value);
    BoxBuilder& equals(Expression expression);
    // State
    const Box* currentBox() const;
    const Port* currentPort() const;
    enum class Amend {Family, Descendants, None} ;
    Box* content(Amend amendOption=Amend::Family);
private:
    // Data
    bool _hasParent;
    Box *_content, *_currentBox;
    Port *_currentPort;
    int _exceptionCount;
    QStack<Box*> _stack;
};

template <class T> BoxBuilder& BoxBuilder::equals(T value) {
    // We need a current to assign value to
    if (!_currentPort)
        ThrowException("BoxBuilder: 'equals' must follow 'port'");
    // For an auxillary port, we did not know its type until now
    if (_currentPort->type() == PortType::Auxiliary)
        _currentPort->initialize(value);
    // Assign the value to the port we are currently defining
    _currentPort->equals(value);
    return *this;
}

}
#endif
