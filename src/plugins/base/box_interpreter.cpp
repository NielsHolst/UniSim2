/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <string>
#include <vector>
#include "ast.h"
#include "box.h"
#include "box_interpreter.h"
#include "convert.h"

namespace base {

BoxInterpreter::BoxInterpreter()
{
}

BoxBuilder BoxInterpreter::interpret() {
    interpret(&ast::_ast);
    return _builder;
}

void BoxInterpreter::interpret(const ast::Node *node) {
    _builder.box(QString::fromStdString(node->className));
    _builder.name(QString::fromStdString(node->objectName));

    for (ast::Parameter parameter : node->parameters) {
        interpret(parameter);
    }
    for (auto child : node->children) {
        const ast::Node *node = child.get_pointer();
        interpret(node);
    }
    _builder.endbox();
}

void BoxInterpreter::interpret(const ast::Parameter &parameter) {
    interpret(parameter.attributedName);
    QString value = QString::fromStdString(parameter.value);
    if (isApostrophed(value))
        _builder.equals(removeApostrophes(value));
    else if (isValue(value))
        _builder.equals(value);
    else
        _builder.imports(value);
}

void BoxInterpreter::interpret(const ast::ParameterWithAttributes &parameterWithAttr) {
    QString name = QString::fromStdString(parameterWithAttr.name);
    if (isAmpersanded(name))
        _builder.newPort(removeAmpersand(name));
    else
        _builder.port(name);

    _attr.clear();
    for (ast::NameValuePair pair : parameterWithAttr.attributes) {
        QString name = QString::fromStdString(pair.name),
                value = QString::fromStdString(pair.value);
        if (isApostrophed(value))
            value = removeApostrophes(value);
        _attr[name] = value;
    }

    if (_attr.contains("label")) _builder.label(_attr.value("label"));
    if (_attr.contains("axis")) _builder.label(_attr.value("axis"));
    if (_attr.contains("track")) setTrack(_attr.value("track"));
}

void BoxInterpreter::setTrack(QString value) {
    if (value == "on")
        _builder.trackOn();
    else if (value == "off")
        _builder.trackOff();
    else
        throw Exception("Track value must be 'on' or 'off'. ", value);
}

bool BoxInterpreter::isAmpersanded(QString s) {
    return s.startsWith("&");
}

QString BoxInterpreter::removeAmpersand(QString s) {
    int n = s.size();
    return s.mid(1, n-1);
}

bool BoxInterpreter::isApostrophed(QString s) {
    if (s.startsWith("\"")) {
        if (s.endsWith("\""))
            return true;
        else
            throw Exception("Unmatched apostrophe in string", s);
    }
    return false;
}

QString BoxInterpreter::removeApostrophes(QString s) {
    int n = s.size();
    return s.mid(1, n-2);
}

bool BoxInterpreter::isValue(QString s) {
    try {
        convert<bool>(s);
        return true;
    }
    catch (Exception &) {}
    try {
        convert<long double>(s);
        return true;
    }
    catch (Exception &) {}
    try {
        convert<QDateTime>(s);
        return true;
    }
    catch (Exception &) {}
    try {
        convert<QTime>(s);
        return true;
    }
    catch (Exception &) {}
    return false;
}

} // namespace
