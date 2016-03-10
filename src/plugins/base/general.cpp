#include <assert.h>
#include <exception.h>
#include "general.h"
#include <QVariant>
namespace base {

void setClassName(QObject *object, QString className_) {
    Q_ASSERT(object);
    if (hasClassName(object)) {
        bool alreadySet = (className(object) == className_);
        if (!alreadySet) {
            object->setProperty("base::Class", QVariant(className_));
            object->setProperty("base::ClassInheritance", QVariant(classInheritance(object) + "/" + className_));
        }
    }
    else {
        object->setProperty("base::Class", QVariant(className_));
        object->setProperty("base::ClassInheritance", QVariant(className_));
    }
}

QString className(const QObject *object) {
    Q_ASSERT(object);
    QVariant className = object->property("base::Class");
    Q_ASSERT(className.isValid());
    return className.toString();
}

QString classInheritance(const QObject *object) {
    if (!object)
        Q_ASSERT(object);
    QVariant classInheritance = object->property("base::ClassInheritance");
    Q_ASSERT(classInheritance.isValid());
    return classInheritance.toString();
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

double accum(const QVector<double> &x) {
    double sum = 0.;
    const double *p = x.data();
    int n = x.size();
    for (int i = 0; i < n; ++i, ++p)
        sum += *p;
    return sum;
}

void increment(QVector<double> &x, const QVector<double> &toAdd) {
    int n = x.size();
    Q_ASSERT(n == toAdd.size());
    increment(const_cast<double*>(x.data()), toAdd.data(), n);
}

void increment(double *x, const double *toAdd, int n) {
    for (int i = 0; i < n; ++i, ++x, ++toAdd)
        *x += *toAdd;
}

} //namespace
