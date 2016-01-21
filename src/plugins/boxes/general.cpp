#include <assert.h>
#include <exception.h>
#include "general.h"
#include <QVariant>
namespace boxes {

void setClassName(QObject *object, QString myClassName) {
    assert(object);
    QString curClassName;
    try {
        curClassName = className(object);
    }
    catch (Exception &) {
    }
    object->setProperty("boxes::Class", QVariant(myClassName));
    object->setProperty("boxes::ClassInheritance", QVariant(curClassName + "/" + myClassName));
}

QString className(const QObject *object) {
    if (!object) return "NULL";
    QVariant className = object->property("boxes::Class");
    if (!className.isValid())
        throw Exception("Object has no class name (did you forget 'Class' in the constructor?)", "", object);
    return className.toString();
}

QString classInheritance(const QObject *object) {
    if (!object) return "NULL";
    QVariant classInheritance = object->property("boxes::ClassInheritance");
    if (!classInheritance.isValid())
        throw Exception("Object has no class name (did you forget 'Class' in the constructor?)", "", object);
    return classInheritance.toString();
}

QString fullName(const QObject *object) {
    if (!object) return "NULL";
    QString name = object->objectName(),
            s = (className(object) == "Port") ? ("[" + name + "]") : ("/" + name);

    const QObject *parent = object->parent();
    return parent ? fullName(parent) + s : s;
}

QStringList split(QString s, QObject *context) {
    QStringList list;
    QString item;
    bool insideAp{false};
    for (int i=0; i < s.size(); ++i) {
        char ch = s.at(i).toLatin1();
        switch (ch) {
        case ' ':
            if (insideAp)
                item += ch;
            else {
                if (!item.isEmpty()) list << item;
                item.clear();
            }
            break;
        case '"':
            if (insideAp || !item.isEmpty()) {
                list << item;
                item.clear();
            }
            insideAp = !insideAp;
            break;
        default:
            item += ch;
        }
    }
    if (insideAp)
        throw Exception("Unmatched apostrophe in string", s, context);
    if (!item.isEmpty()) list << item;
    return list;
}

QStringList split(const char *s, QObject *context) {
    return split(QString(s), context);
}

} //namespace
