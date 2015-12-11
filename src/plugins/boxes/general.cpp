#include <assert.h>
#include <exception.h>
#include "general.h"
#include <QVariant>
namespace boxes {

void setClassName(QObject *object, QString className) {
    assert(object);
    object->setProperty("boxes::Class", QVariant(className));
}

QString className(const QObject *object) {
    if (!object) return "NULL";
    QVariant className = object->property("boxes::Class");
    if (!className.isValid())
        throw Exception("Object has no class name (did you forget ID in the constructor?)", "", object);
    return className.toString();
}

QString fullName(const QObject *object) {
    if (!object) return "NULL";
    QString name = object->objectName(),
            s = (className(object) == "Port") ? ("[" + name + "]") : ("/" + name);

    const QObject *parent = object->parent();
    return parent ? fullName(parent) + s : s;
}

} //namespace
