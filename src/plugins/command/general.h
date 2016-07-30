#ifndef COMMAND_GENERAL
#define COMMAND_GENERAL

#include <QObject>
#include <QVector>

namespace command {

QString info(QVector<QObject*> objects);
QString info(QObject *object);

}

#endif
