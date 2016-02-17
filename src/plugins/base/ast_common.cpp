#include <QMap>
#include <QMapIterator>
#include "ast_common.h"
#include "convert.h"

using namespace base;
using boost::optional;

namespace {

    bool isAmpersanded(QString s) {
        return s.startsWith("&");
    }

    QString removeAmpersand(QString s) {
        int n = s.size();
        return s.mid(1, n-1);
    }

    bool isApostrophed(QString s) {
        if (s.startsWith("\"")) {
            if (s.endsWith("\""))
                return true;
            else
                throw Exception("Unmatched apostrophe in string", s);
        }
        return false;
    }

    QString removeApostrophes(QString s) {
        int n = s.size();
        return s.mid(1, n-2);
    }

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

//    struct Attributes : public QMap<QString, optional<QString>> {

//        Attributes() : QMap<QString, optional<QString>>()
//        {
//            _attributes["axis"]  = optional<QString>();
//            _attributes["group"] = optional<QString>();
//            _attributes["label"] = optional<QString>();
//            _attributes["page"]  = optional<QString>();
//            _attributes["track"] = optional<QString>();
//        }
//    };

//    typedef QMapIterator<QString, optional<QString>> AttributesIterator;
}

namespace ast {

QString NameValuePair::toString() const {
    return QString::fromStdString(name + "~" + value);
}

QString ParameterWithAttributes::toString() const {
    QString s = QString::fromStdString(name);
    if (attributes.size() > 0) {
        s += " with";
        for (auto attr : attributes)
            s += " " + attr.toString();
    }
    return s;
}

void ParameterWithAttributes::addToBuilder(base::BoxBuilder &builder) {
    QString nam = QString::fromStdString(name);
    if (isAmpersanded(nam))
        builder.newPort(removeAmpersand(nam));
    else
        builder.port(nam);

//    Attributes attr;
    for (NameValuePair pair : attributes) {
        QString name = QString::fromStdString(pair.name),
                value = QString::fromStdString(pair.value);
        if (isApostrophed(value))
            value = removeApostrophes(value);
        builder.attribute(name, value);
//        if (attr.contains(name))
//            attr[name] = value;
//        else
//            throw Exception("Unknown port attribute", name);
    }

//    AttributesIterator it(attr);
//    while (it.hasNext()) {
//        it.next();
//        QString &name(it.key());
//        optional<QString> &value(it.value());
//        if (value.is_initialized())
//            _builder.attribute(name, value.get());
//    }
}

QString Parameter::toString() const {
    QString val = value.is_initialized() ?
                  QString::fromStdString(value.get()) :
                  QString("na");
    return attributedName.toString() + " equals '" + val + "'";
}

void Parameter::addToBuilder(base::BoxBuilder &builder) {
    attributedName.addToBuilder(builder);
    if (value.is_initialized()) {
        QString val = QString::fromStdString(value.get());
        if (isApostrophed(val))
            builder.equals(removeApostrophes(val));
        else if (isValue(val))
            builder.equals(val);
        else
            builder.imports(val);
    }
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

