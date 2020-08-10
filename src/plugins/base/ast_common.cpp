/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <iostream> // test
#include <QMap>
#include <QMapIterator>
#include "ast_common.h"
#include "convert.h"
#include "general.h"

using namespace base;
using boost::optional;

namespace {

    bool isValue(QString s) {
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
}

namespace ast {

QString ParameterWithAttribute::toString() const {
    QString s = QString::fromStdString(type) + QString::fromStdString(name);
    if (attribute.size() > 0)
        s += "|" + QString::fromStdString(attribute);
    return s;
}

void ParameterWithAttribute::addToBuilder(base::BoxBuilder &builder) {
//    QString s("type '%1', name '%2', assignment '%3', attribute '%4'");
//    dialog().information(s.
//                         arg(QString::fromStdString(type)).
//                         arg(QString::fromStdString(name)).
//                         arg(QString::fromStdString(assignment)).
//                         arg(QString::fromStdString(attribute)));
    QString nam = QString::fromStdString(name);
    if (type==".")
        builder.port(nam);
    else {
        Q_ASSERT(type=="+");
        builder.newPort(nam);
    }
    QString transform = QString::fromStdString(attribute);
    builder.attribute("transform", transform);

}

QString Parameter::toString() const {
    QString val = QString::fromStdString(value),
            s = attributedName.toString() + " equals '" + val + "'";
    return s;
}

void Parameter::addToBuilder(base::BoxBuilder &builder) {
    attributedName.addToBuilder(builder);
    QString val = QString::fromStdString(value),
            dist,
            fallBackValue;
//    dialog().information("value '" + val + "'");
    QString assignment = val.at(0);
    val = val.mid(1);

    if (val.startsWith("?")) {
        int pos = val.indexOf(":");
        Q_ASSERT(pos>1);
        fallBackValue = val.mid(pos+1);
        val = val.mid(1, pos-1);
    }
    if (val.contains("@") && !isApostrophed(val) && !isParenthesized(val)) {
        int pos = val.indexOf("@");
        dist = deEmbrace(val.mid(pos+1));
        val = val.left(pos);
    }

    if (!fallBackValue.isNull()) {
        builder.importsMaybe(val, fallBackValue);
    }
    else if (isApostrophed(val)) {
        val = deEmbrace(val);
        if (isParenthesized(val))
            builder.equals(split(val));
        else
            builder.equals(val);
    }
    else if (isParenthesized(val))
        builder.equals(split(val));
    else if (isValue(val))
        builder.equals(val);
    else
        builder.imports(val);
    if (!dist.isNull())
        builder.rnd(dist);
}

void Node::clear() {
    className.clear();
    objectName.clear();
    parameters.clear();
    children.clear();
}

QString Node::toString() const {
    QString s;
    addToString(s, 0);
    return s;
}

void Node::addToString(QString &s, int level) const {
    s += QString().fill(' ', 2*level);
    s += QString::fromStdString(className + " " + objectName);
    s += "\n";
    for (Parameter parameter : parameters) {
        s += QString().fill(' ', 2*(level+1));
        s += parameter.toString() + "\n";
    }
    for (auto child : children) {
        const Node *node = child.get_pointer();
        node->addToString(s,level+1);
    }
}

void Node::addToBuilder(base::BoxBuilder &builder) {
    builder.box(QString::fromStdString(className));
    builder.name(QString::fromStdString(objectName));
    for (Parameter parameter : parameters)
        parameter.addToBuilder(builder);
    for (auto child : children) {
        Node *node = child.get_pointer();
        node->addToBuilder(builder);
    }
    builder.endbox();
}

}

