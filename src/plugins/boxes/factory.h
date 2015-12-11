#ifndef FACTORY_H
#define FACTORY_H

#include <boxes/factory_plug_in.h>

namespace BOXES_PLUGIN_NAME {

class Factory : public QObject, public boxes::FactoryPlugIn
{
Q_OBJECT
Q_INTERFACES(boxes::FactoryPlugIn)
Q_PLUGIN_METADATA(IID "org.ecolmod.boxes")
public:
    QString id() const;
    QList<QString> inventory();
    QObject* create(QString className, QString objectName, QObject *parent);
};

} //namespace

#endif
