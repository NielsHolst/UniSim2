/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_SAVE_GRAPH_BASE_H
#define BASE_SAVE_GRAPH_BASE_H
#include <QFile>
#include <QObject>
#include <QPair>
#include <QSet>
#include <QTextStream>

namespace base {

class Box;
class Port;

class SaveGraphBase : public QObject
{
public:
    SaveGraphBase();
    void writeGraphFile();
    virtual void showGraphFile() = 0;
protected:
    // data
    QFile _file;
    QTextStream _stream;
    // methods
    QVector<Box*> boxesToGraph();
    QVector<QPair<Port*, Port*>> portEdgesToGraph();
private:
    // methods
    virtual void openFileStream() = 0;
    virtual void closeFileStream() = 0;
    virtual void writeBeginning() = 0;
    virtual void writeBoxes() = 0;
    virtual void writeBoxRelations() = 0;
    virtual void writePortEdges() = 0;
    virtual void writeEnd() = 0;
};

}

#endif
