/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_BOX_OUTPUT_H
#define BASE_BOX_OUTPUT_H

#include <QObject>
#include <QStack>
#include <QString>

namespace base {

class Box;
class Port;

class BoxOutput
{
public:
    enum Format{Indented, List};
    BoxOutput(const Box *box, Format format);
    QString asText();
private:
    const Box *_box;
    Format _format;
    int _level;
    QString _output;

    void addOutput(const QObject *box);
    QString entry(const QObject *object);
    QString portValue(const Port *port);
    QString beforeEntry();
    QString afterEntry();
    QString indent();

    QString beginLevel();
    QString endLevel();
};

}
#endif
