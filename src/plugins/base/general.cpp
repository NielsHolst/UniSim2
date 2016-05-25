#include <assert.h>
#include <exception.h>
#include "general.h"
#include <QVariant>
namespace base {

//! Namespaces are currently ignored !

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
    return className.isValid() ? className.toString() : QString("QObject");
}

QString classInheritance(const QObject *object) {
    Q_ASSERT(object);
    QVariant classInheritance = object->property("base::ClassInheritance");
    return classInheritance.isValid() ? classInheritance.toString() : QString("QObject");
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
    // Remove embracing parentheses
    s = s.trimmed();
    if (s.left(1) == "(" && s.right(1) == ")") {
        int n = s.size();
        s = s.left(n-1).right(n-2);
    }
    // Split
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
        ThrowException("Unmatched apostrophe in string").value(s).context(context);
    if (!item.isEmpty()) list << item;
    return list;
}

QStringList split(const char *s, QObject *context) {
    return split(QString(s), context);
}

bool isApostrophed(QString s) {
    if (s.startsWith("\"")) {
        if (s.endsWith("\""))
            return true;
        else
            ThrowException("Unmatched apostrophe in string").value(s);
    }
    return false;
}

bool isParenthesized(QString s) {
    if (s.startsWith("(")) {
        if (s.endsWith(")"))
            return true;
        else
            ThrowException("Unmatched parenthesis in string").value(s);
    }
    return false;
}

QString deEmbrace(QString s) {
    int n = s.size();
    return s.mid(1, n-2);
}

QString locateFile(QDir baseDir, QDir specificDir, QString filePath) {
    if (QDir(filePath).isAbsolute())
        return filePath;
    QDir dir = locateDir(baseDir, specificDir);

    QString myFilePath = filePath;
    if (filePath.startsWith("...")) {
        myFilePath = filePath.mid(4);
        while (!dir.exists(myFilePath) && dir.cdUp());
    }

    if (!dir.exists(myFilePath)) {
        QString msg{"Could not find '%1' in '%2'"};
        ThrowException(msg.arg(filePath).arg(locateDir(baseDir, specificDir).absolutePath()));
    }
    return dir.absolutePath() + "/" + myFilePath;
}

QDir locateDir(QDir baseDir, QDir specificDir) {
    if (!baseDir.isAbsolute())
        ThrowException("Base directory must be an absolute path").value(baseDir.path());

    QString path = (specificDir.isAbsolute()) ?
                specificDir.absolutePath() :
                (baseDir.absolutePath() + "/" + specificDir.path());
    QDir dir(path);
    return dir;
}

QDir makeDir(QDir baseDir, QDir specificDir) {
    QDir dir = locateDir(baseDir, specificDir);
    if (!dir.mkpath("."))
        ThrowException("Could not create folder").value(dir.path());
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
