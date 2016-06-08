#include <algorithm>
#include <QDesktopServices>
#include <QFileInfo>
#include <QMapIterator>
#include <QProcess>
#include <QSet>
#include <QUrl>
#include "box.h"
#include "dialog.h"
#include "environment.h"
#include "exception.h"
#include "port.h"
#include "save_graph_graphviz.h"

namespace base {

inline QString embrace(QString s) {
    return "\"" + s + "\"";
}

inline QString id(const Box *box) {
    return QString{"n%1"}.arg(box->order());
}
inline QString id(const Port *port) {
    const Box *parent = dynamic_cast<const Box*>(port->parent());
    return id(parent) + port->objectName();
}

SaveGraphGraphviz::SaveGraphGraphviz() {
}

void SaveGraphGraphviz::openFileStream() {
    environment().openOutputFile(_file, "dot");
    _stream.setDevice(&_file);
}

void SaveGraphGraphviz::writeBeginning() {
    _portEdgesToGraph = portEdgesToGraph();
    _stream << "digraph G {\n"
            << "size=\"11.69,16.53\";\n"    // A3 size
            << "ranksep=1;\n"
            << "graph[rankdir=LR];\n"
            << "node[shape=Mrecord];\n";
}

void SaveGraphGraphviz::writeEnd() {
    collectNodeDepths();
    QMapIterator<int, QStringList> i(_nodeDepths);
    while (i.hasNext()) {
        i.next();
        _stream << "{ rank=same; " << i.value().join(";") << "}\n";
    }
    _stream << "}\n";
}

inline QString label(const Box* box) {
    return box->objectName().isEmpty() ? QString() : "\\n" + box->objectName();
}

inline QString label(const Port *port) {
    return "<" + port->objectName() + "> ." + port->objectName();
}

QList<Port*> SaveGraphGraphviz::uniquePortsToGraph(const Box *box) {
    QSet<Port*> ports;
    for (QPair<Port*, Port*> edge : _portEdgesToGraph) {
        if (box==edge.first->parent())
            ports << edge.first;
        else if (box==edge.second->parent())
            ports << edge.second;
    }
    QList<Port*> sorted = ports.toList();
    std::sort(sorted.begin(), sorted.end(),
              [](Port *a, Port *b) { return a->objectName() < b->objectName(); });
    return sorted;
}

void SaveGraphGraphviz::writeBoxes() {
    QVector<Box*> boxes = boxesToGraph();
    if (boxes.isEmpty())
        writeNode("n0", "No boxes to graph");
    for (const Box *box : boxes) {
        QStringList fields;
        fields << box->className() + label(box);
        for (Port *port : uniquePortsToGraph(box))
            fields << label(port);
        writeNode(id(box), fields.join("|"));
    }
}

void SaveGraphGraphviz::writeBoxRelations() {
    _stream << "edge[style=\"setlinewidth(8)\", color=gray];\n";
    QVector<Box*> parents = boxesToGraph();
    for (Box *parent : parents) {
        QVector<Box*> children = parent->resolveMany<Box>("./*");
        for (Box *child : children) {
            writeEdge(id(parent), id(child));
        }
    }
}

inline QString edgeId(const Box *box, const Port *port) {
    return id(box) + ":" + port->objectName();
}

void SaveGraphGraphviz::writePortEdges() {
    _stream << "edge[style=\"setlinewidth(1)\", color=blue];\n";
    for (QPair<Port*, Port*> edge : portEdgesToGraph()) {
        Port *portSender = edge.first,
             *portReceiver = edge.second;
        Box *boxSender = dynamic_cast<Box*>(portSender->parent()),
            *boxReceiver = dynamic_cast<Box*>(portReceiver->parent());
        writeEdge(edgeId(boxSender, portSender), edgeId(boxReceiver, portReceiver));
    }
}

void SaveGraphGraphviz::writeEdge(QString from, QString to, QString arrowHead) {
    _stream << from << "->" << to;
    if (arrowHead != "normal")
        _stream << "[arrowhead=" << arrowHead << "]";
    _stream << ";\n";

}

void SaveGraphGraphviz::writeNode(QString id, QString label) {
    _stream <<  id
            << "[label=" << embrace(label)
            << "];\n";
}

inline int depth(const Box *box) {
    int i(0);
    QObject *p = const_cast<QObject*>( dynamic_cast<const QObject*>(box) );
    while (p && p != environment().state.root) {
        p = p->parent();
        ++i;
    }
    return i;
}

void SaveGraphGraphviz::collectNodeDepths() {
    _nodeDepths.clear();
    for (const Box *box : boxesToGraph())
        _nodeDepths[depth(box)] << id(box);
}

void SaveGraphGraphviz::closeFileStream() {
    dialog().information("graph script written to " + environment().latestOutputFilePath("dot"));
    _file.close();
}

void SaveGraphGraphviz::showGraphFile() {
    writeBatchFile();
    runBatchFile();
    QString svgFileName = environment().latestOutputFilePath("svg");
    if (QFileInfo(svgFileName).exists()) {
        dialog().information("graph written to " + svgFileName);
        writeHtmlFile();
        showHtmlFile();
    }
    else
        dialog().error("error generating svg graph file:\n" + svgFileName + "\n" + processMessage());
}

QString SaveGraphGraphviz::processMessage() {
    return QString(process->readAllStandardError());
}

void SaveGraphGraphviz::writeBatchFile() {
    QFile batch;
    environment().openOutputFile(batch, "bat");

    QTextStream s(&batch);
    s << commandAndArg().join(" ") << "\n";
    batch.close();
}

void SaveGraphGraphviz::runBatchFile() {
    QString batchFilePath = environment().latestOutputFilePath("bat");
    process = new QProcess(this);

    process->start(embrace(batchFilePath), QStringList());

    if (process->waitForStarted()) {
        dialog().information("working...");
        if (process->waitForFinished())
            dialog().information("done");
        else
            dialog().error("dot program timed out; not finished");
    }
    else {
        dialog().error("dot program timed out; not run\n" + processMessage());
    }
}

QStringList SaveGraphGraphviz::commandAndArg() {
    QString dotFilePath = environment().latestOutputFilePath("dot"),
            outputFilePath = environment().outputFilePath("svg");
    return QStringList()
            << embrace(dotCommand())
            << " -T svg " + embrace(dotFilePath) +  " -o " +  embrace(outputFilePath);
}

QString SaveGraphGraphviz::dotCommand() {
//Maybe only #ifdef Q_OS_WIN32 ?
QDir dir = environment().dir(Environment::Graphviz);
QString filePath1 = dir.absoluteFilePath("dot.exe"),
        filePath2 = dir.absoluteFilePath("bin/dot.exe"),
        filePath3 = "dot";
bool ok1 = QFile::exists(filePath1),
     ok2 = QFile::exists(filePath2);
return ok1 ? filePath1 : ok2 ? filePath2 : filePath3;
}

void SaveGraphGraphviz::writeHtmlFile() {
    QFile file;
    environment().openOutputFile(file, "html");
    QTextStream stream(&file);

    QString html = htmlCode(),
            svgFileName = environment().latestOutputFilePath("svg");
    html.replace("#title", QFileInfo(svgFileName).baseName());
    html.replace("#svg-file-name", svgFileName);
    stream << html;
    file.close();
}

QString SaveGraphGraphviz::htmlCode() {
    QFile file(":svg_template3.html");
    if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) )
        ThrowException("Cannot open file").context(this);

    QString s;
    QTextStream stream(&s);
    stream << file.readAll();
    return s;
}

void SaveGraphGraphviz::showHtmlFile() {
    QString url = "file:///" + environment().latestOutputFilePath("html");
    bool ok = QDesktopServices::openUrl(QUrl(url));
    if (ok)
        dialog().information("graph loaded by browser");
    else
        dialog().error("graph could not be loaded by browser; no HTML browser set?");
}

}
