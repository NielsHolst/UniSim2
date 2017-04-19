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

QString NameValuePair::toString() const {
    return QString::fromStdString(name + "~" + value);
}

QString ParameterWithAttributes::toString() const {
    QString s = QString::fromStdString(type) + QString::fromStdString(name);
    if (attributes.size() > 0) {
        s += " with";
        for (auto attr : attributes)
            s += " " + attr.toString();
    }
    return s;
}

void ParameterWithAttributes::addToBuilder(base::BoxBuilder &builder) {
    QString nam = QString::fromStdString(name);
    if (type==".")
        builder.port(nam);
    else {
        Q_ASSERT(type=="+");
        builder.newPort(nam);
    }

    for (NameValuePair pair : attributes) {
        QString name = QString::fromStdString(pair.name),
                value = QString::fromStdString(pair.value);
        if (isApostrophed(value))
            value = deEmbrace(value);
        builder.attribute(name, value);
    }

}

QString Parameter::toString() const {
    QString val = QString::fromStdString(value),
            s = attributedName.toString() + " equals '" + val + "'";
    return s;
}

void Parameter::addToBuilder(base::BoxBuilder &builder) {
    attributedName.addToBuilder(builder);
    QString val = QString::fromStdString(value),
            dist;
    if (val.contains("@") && !isApostrophed(val) && !isParenthesized(val)) {
        int pos = val.indexOf("@");
        dist = deEmbrace(val.mid(pos+1));
        val = val.left(pos);
    }

    if (isApostrophed(val)) {
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

