/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BOX_INTERPRETER_H
#define BOX_INTERPRETER_H

#include <QMap>
#include <QString>
#include "box_builder.h"
#include "box_reader_base.h"

namespace ast {
    class Node;
    class Parameter;
    class ParameterWithAttributes;
}

namespace base {

class BoxInterpreter
{
public:
    BoxInterpreter();
    BoxBuilder interpret();
private:
    // Data
    BoxBuilder _builder;
    QMap<QString, QString> _attr;
    // Methods
    void interpret(const ast::Node *node);
    void interpret(const ast::Parameter &parameter);
    void interpret(const ast::ParameterWithAttributes &parameterWithAttr);
    void setValue();
    void setTrack(QString value);
    static bool isAmpersanded(QString s);
    static QString removeAmpersand(QString s);
    static bool isApostrophed(QString s);
    static QString removeApostrophes(QString s);
    static bool isValue(QString s);
};

}

#endif
