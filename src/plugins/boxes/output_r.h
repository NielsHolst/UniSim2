#ifndef OUTPUT_R_H
#define OUTPUT_R_H
#include <QFile>
#include <QString>
#include <base/box.h>

class QFile;

namespace base {
    class Port;
}

namespace boxes {

class OutputR : public base::Box
{
public:
    OutputR(QString name, QObject *parent);
    void initialize();
    void debrief();
    QString toString();
    QString toScript();
private:
    // Data
    QFile _file;

    struct PageInfo;
    struct PlotInfo;

    QVector<PageInfo> _pageInfos;

    struct PageInfo {
        const Box *_page;
        QVector<PlotInfo> _plotInfos;
        // Methods
        PageInfo(){}
        PageInfo(Box *page);
        QString toString();
        QString toScript();
    };

    struct PlotInfo {
        const Box *_plot;
        QVector<const base::Port*> _ports;
        // Methods
        PlotInfo(){}
        PlotInfo(Box *plot);
        void collectPorts();
        QString toString();
        QString toScript();
        QString xPortLabel();
    };

    // Methods
    void collectInfo();
    QVector<const base::Port*> collectPorts(QString pageName, QString plotName);

    void writeScript();
    void openFile(QString filePath);
    QString scriptFilePath();
    QString scriptFolderPath();
    QString fileName();

};

}

#endif
