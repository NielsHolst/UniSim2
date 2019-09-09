/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QVariant>
#include <assert.h>
#include <exception.h>
#include "environment.h"
#include "general.h"

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
    QString name = object->objectName().isEmpty() ? "*" : object->objectName(),
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
        case '\t':
        case '\n':
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

bool isPath(QString s) {
    char first = (s.size()>0) ? s.at(0).toLatin1() : 0,
         second = (s.size()>1) ? s.at(1).toLatin1() : 0;
    return ('A'<=first && first<='Z') ||
           ('a'<=first && first<='z') ||
           ('*'==first) ||
           ('/'==first) ||
           ('.'==first && '.'==second) ||
           ('.'==first && second<='0') ||
           ('.'==first && second>='9');
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

//! Returns the full path for the file with given file name
/*! The file is searched in the order:
 *  home/fileName
 *  home/subFolder/fileName
 *  home/../fileName
 *  home/../subFolder/fileName
 *  home/../../fileName
 *  home/../../subFolder/fileName
 *  etc.
 *  If the file is not found, the first file path above is returned.
*/
//QFileInfo findNearestFile(QDir home, QString subFolder, QString fileName) {
//    QDir dir = home;
//    while (true) {
//        QString filePath[2];
//        filePath[0] = dir.absolutePath() + "/" + fileName;
//        filePath[1] = dir.absolutePath() + "/" + subFolder + "/" + fileName;
//        if (QFileInfo(filePath[0]).exists()) {
//            break;
//        }
//        if (QFileInfo(filePath[1]).exists()) {
//            dir.cd(subFolder);
//            break;
//        }
//        if (!dir.cdUp()) {
//            dir = home;
//            break;
//        }
//    }
//    return QFileInfo(dir.filePath(fileName));
//}

} //namespace
