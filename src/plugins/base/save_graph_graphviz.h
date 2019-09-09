/* Copyright 2005-2019 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_SAVE_GRAPH_GRAPHVIZ_H
#define BASE_SAVE_GRAPH_GRAPHVIZ_H
#include <QFile>
#include <QList>
#include <QMap>
#include <QTextStream>
#include "save_graph_base.h"

class QProcess;

namespace base {

class Box;
class Port;

class SaveGraphGraphviz : public SaveGraphBase
{
public:
    SaveGraphGraphviz();
    void showGraphFile();
private:
    // data
    QProcess *process;
    QVector<QPair<Port*, Port*>> _portEdgesToGraph;
    QMap<int, QStringList> _nodeDepths;
    // methods
    void openFileStream();
    void closeFileStream();
    void writeBeginning();
    void writeBoxes();
    void writeBoxRelations();
    void writePortEdges();
    void writeEnd();
    void writeNode(QString id, QString label);
    void writeEdge(QString from, QString to, QString arrowHead="normal");
    void collectNodeDepths();
    QList<base::Port *> uniquePortsToGraph(const Box *box);

    void writeBatchFile();
    void runBatchFile();
    void writeHtmlFile();
    void showHtmlFile();
    void generate();
    QString processMessage();

    void generateByWindowsBatchFile();
    QStringList commandAndArg();

    QString dotCommand();
    QString htmlCode();
};

}

#endif
