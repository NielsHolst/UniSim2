#include <assert.h>
#include <exception.h>
#include "general.h"
#include <QVariant>
namespace base {

void setClassName(QObject *object, QString myClassName) {
    Q_ASSERT(object);
    QString curClassName = hasClassName(object) ? className(object) : QString();
    object->setProperty("base::Class", QVariant(myClassName));
    object->setProperty("base::ClassInheritance", QVariant(curClassName + "/" + myClassName));
}

QString className(const QObject *object) {
    if (!object) return "NULL";
    QVariant className = object->property("base::Class");
//    if (!className.isValid())
//        throw Exception("Object has no class name (did you forget 'Class' in the constructor?)", "", object);
    return className.isValid() ? className.toString() : QString("UnknownClass");
}

QString classInheritance(const QObject *object) {
    if (!object) return "NULL";
    QVariant classInheritance = object->property("base::ClassInheritance");
//    if (!classInheritance.isValid())
//        throw Exception("Object has no class name (did you forget 'Class' in the constructor?)", "", object);
    return classInheritance.isValid() ? classInheritance.toString() : QString("UnknownClass");
}

bool hasClassName(const QObject *object) {
    return object->property("base::Class").isValid();
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

QDir locateDir(QDir baseDir, QDir specificDir) {
    if (!baseDir.isAbsolute())
        throw Exception("Base directory must be an absolute path", baseDir.path());

    QString path = (specificDir.isAbsolute()) ?
                specificDir.absolutePath() :
                (baseDir.absolutePath() + "/" + specificDir.path());
    QDir dir(path);
    return dir;
}

QDir makeDir(QDir baseDir, QDir specificDir) {
    QDir dir = locateDir(baseDir, specificDir);
    if (!dir.mkpath("."))
        throw Exception("Could not create folder", dir.path());
    return dir;
}

} //namespace
