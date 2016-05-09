#ifndef OUTPUT_FILE_H
#define OUTPUT_FILE_H

#include <QFile>
#include <QStringList>

class OutputFile
{
public:
    OutputFile();
    QString filePath();
    QStringList columnLabels();
    QStringList columnFormats();
    int numRows();
    QString cell(int i, QString col);
    QStringList row(int i);
    QStringList row(int i, QStringList columns);
    QStringList column(int i);
    QStringList column(QString label);
private:
    void open();
    void close();
    QFile file;
};

#endif
