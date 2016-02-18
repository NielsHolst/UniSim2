#ifndef GENERAL_H
#define GENERAL_H
#include <QDir>
#include <QObject>
#include <QString>

#define Class(x) setClassName(this, #x)

namespace base {

void setClassName(QObject *object, QString className);
QString className(const QObject *object);
QString classInheritance(const QObject *object);
bool hasClassName(const QObject *object);

QString fullName(const QObject *object);

QStringList split(QString s, QObject *context = 0);
QStringList split(const char *s, QObject *context = 0);

QDir locateDir(QDir baseDir, QDir specificDir);
QDir makeDir(QDir baseDir, QDir specificDir);

} //namespace

#endif
