#ifndef GENERAL_H
#define GENERAL_H
#include <QObject>
#include <QString>

#define Class(x) setClassName(this, #x)

namespace boxes {

void setClassName(QObject *object, QString className);
QString className(const QObject *object);
QString classInheritance(const QObject *object);

QString fullName(const QObject *object);

} //namespace

#endif
