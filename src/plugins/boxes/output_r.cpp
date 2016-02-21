#include <QDir>
#include <QTextStream>
#include <base/environment.h>
#include <base/exception.h>
#include <base/general.h>
#include <base/path.h>
#include <base/port.h>
#include <base/publish.h>
#include "output_r.h"

using namespace base;

namespace boxes {

PUBLISH(OutputR)

OutputR::OutputR(QString name, QObject *parent)
    : Box(name, parent)
{
    Class(OutputR);
}

//
// initialize
//

void OutputR::initialize() {
    try {
        collectInfo();
    }
    catch (Exception &ex) {
        QString msg{"Unexpected error in OutputR::collectInfo:\n"};
        throw( Exception(msg+ex.what(), "", this) );
    }
}

void OutputR::collectInfo() {
    QVector<Box*> pages = Path("children::*{PageR}", this).resolve<Box>();
    for (Box *page : pages)
        _pageInfos << PageInfo(page);
}

QString OutputR::toString() {
    QString s;
    for (PageInfo pageInfo : _pageInfos)
        s += pageInfo.toString();
    return s;
}

QString OutputR::toScript() {
    QString s;
    for (PageInfo pageInfo : _pageInfos)
        s += pageInfo.toScript();
    return s;
}

OutputR::PageInfo::PageInfo(Box *page)
    : _page(page)
{
    QVector<Box*> plots = Path("children::*{PlotR}", page).resolve<Box>();
    for (Box *plot : plots)
        _plotInfos << PlotInfo(plot);
}

QString OutputR::PageInfo::toString() {
    QString s = _page->className() + " " + _page->objectName() + "\n";
    for (PlotInfo plotInfo : _plotInfos)
        s += plotInfo.toString();
    return s;
}

QString OutputR::PageInfo::toScript() {
    QString string;
    QTextStream s(&string);
    s << _page->objectName() << " = function(df) {\n"
      << "  windows("
      << _page->port("width")->value<int>()
      << ", "
      << _page->port("height")->value<int>()
      << ")\n"
      << "  grid.arrange(\n" ;
    for (PlotInfo plotInfo : _plotInfos)
        s << plotInfo.toScript();
    s << "    nrow = " << _page->port("nrow")->value<int>() << "\n  )\n}\n";
    return string;
}

OutputR::PlotInfo::PlotInfo(Box *plot)
    : _plot(plot)
{
    collectPorts();
}

void OutputR::PlotInfo::collectPorts() {
    Box *parent = dynamic_cast<Box*>(_plot->parent());
    Q_ASSERT(parent);
    QString pageName = parent->objectName(),
            plotName = _plot->objectName();

    QVector<base::Port*> all = Path("*{Port}").resolve<Port>();
    for (const Port *port : all) {
        if (port->page() == pageName && port->plot() == plotName && port->trackPtr())
            _ports << port;
    }
}

QString OutputR::PlotInfo::toString() {
    QString s = "Plot: " + _plot->objectName() + "\n"; //" " + _parent->test +
    for (const Port *port : _ports)
        s += "  Port: " + port->objectName() + "\n";
    return s;
}

inline QString apostrophed(QString s) {
    return "\"" + s + "\"";
}

QString OutputR::PlotInfo::toScript() {
    QStringList portLabels ;
    portLabels << apostrophed(xPortLabel());
    for (const Port *port : _ports)
        portLabels << apostrophed(port->label());
    return "    unisim_plot(df, c(" + portLabels.join(", ") + ")),\n";
}

QString OutputR::PlotInfo::xPortLabel() {
    Box *parent = dynamic_cast<Box*>(_plot->parent());
    Q_ASSERT(parent);
    QString xPath = parent->port("xPath")->value<QString>();
    Port *xPort = Path(xPath).resolveOne<Port>();
    return xPort->label();
}

//
// debrief
//

void OutputR::debrief() {
    writeScript();
}

void OutputR::writeScript() {
    QString filePath = scriptFilePath();
    openFile(filePath);
    QTextStream script(&_file);
    script << toScript();
    _file.close();
}

void OutputR::openFile(QString filePath) {
    _file.setFileName(filePath);
    if ( !_file.open(QIODevice::WriteOnly | QIODevice::Text) )
        throw Exception("Cannot open file for output", filePath, this);
}

QString OutputR::fileName() {
    QString objectName = environment().state.root->objectName(),
            className  = environment().state.root->className();
    return objectName.isEmpty() ? className : objectName;
}

QString OutputR::scriptFilePath() {
    QString filePath = scriptFolderPath() + "/" + fileName() + "_outputR.R";
    filePath.replace("\\", "/");
    return filePath;
}

QString OutputR::scriptFolderPath() {
    QDir dir = makeDir(environment().state.dir.work, environment().state.dir.script);
    QString folderPath = dir.absolutePath();
    folderPath.replace("\\", "/");
    return folderPath;
}


}
