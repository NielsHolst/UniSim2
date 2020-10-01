/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_MEGA_FACTORY_H
#define BASE_MEGA_FACTORY_H

#include <QDir>
#include <QMultiMap>
#include <QObject>
#include "exception.h"
#include "general.h"

namespace base {
	
class Box;
class FactoryPlugIn;

class MegaFactory : public QObject
{
public:
    static void loadPlugins();

    template <class T=Box>
    static T* create(QString className, QString objectName, QObject *parent);

    static const QList<base::FactoryPlugIn*> &factories();
    static QDir pluginsDir();
    static QStringList qualifiedClassNames(QString className);
    static void usingPlugin(QString pluginName);
private:
    // methods
    MegaFactory();
    static MegaFactory* me();
    static QObject* createObject(QString className, QString objectName, QObject *parent=0);
    // data
    typedef QMultiMap<QString, FactoryPlugIn*> ProductIndex;
    static MegaFactory *_me;
    ProductIndex productIndex;
    QList<base::FactoryPlugIn*> _factories;
    static QString _usingPluginName;
};

template <class T>
T* MegaFactory::create(QString className, QString objectName, QObject *parent)
{
    QObject *object = MegaFactory::createObject(className, objectName, parent);
    QString msg = QString("MegaFactory cannot create object '%1' of class '%2'").arg(objectName).arg(className);
    if (!object)
        ThrowException(msg);
    setClassName(object, className);
    T *specificObject = dynamic_cast<T*>(object);
    if (!object)
        ThrowException(msg);
    return specificObject;
}


}//namespace

#endif
