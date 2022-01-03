/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QVariant>
#include <exception.h>
#include "class_name.h"

namespace base {

inline QString bare(const QObject *object, QString className) {
    QStringList names = className.split("::");
    if (names.size() != 2)
        ThrowException("Illegal class name").value(className).context(object);
    return names.at(1);
}

void addClassName(QObject *object, QString className) {
    if (!object)
        ThrowException("Object pointer is null");
    if (!className.contains("::"))
        ThrowException("Class name must include namespace").value(className).context(object);

    QStringList classNames, bareClassNames;
    if (object->property("ClassNames").isValid())
        classNames = object->property("ClassNames").toStringList();
    if (object->property("BareClassNames").isValid())
        bareClassNames = object->property("BareClassNames").toStringList();

    QStringList names = className.split("::");
    if (names.size() != 2)
        ThrowException("Illegal class name").value(className).context(object);

    classNames     << className;
    bareClassNames << bare(object, className);
    object->setProperty("ClassNames",     classNames);
    object->setProperty("BareClassNames", bareClassNames);
}

bool hasClassName(const QObject *object, QString className) {
    if (!object)
        ThrowException("Object pointer is null");
    if (!object->property("ClassNames").isValid())
        return false;
    return object->property("ClassNames").toStringList().contains(className) ||
           object->property("BareClassNames").toStringList().contains(bare(object,className));

}
} //namespace
