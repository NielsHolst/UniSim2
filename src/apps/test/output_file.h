#ifndef OUTPUT_FILE_H
#define OUTPUT_FILE_H

#include <QFile>
#include <QStringList>
#include <base/convert.h>

class OutputFile
{
public:
    OutputFile();
    QString filePath();
    QStringList columnLabels();
    QStringList columnFormats();
    int numRows();
    template <class T=QString> T cell(int i, QString col);
    QStringList row(int i);
    QStringList row(int i, QStringList columns);
    QStringList column(int i);
    QStringList column(QString label);
private:
    void open();
    void close();
    QFile file;
};

template <class T>
T OutputFile::cell(int i, QString col) {
    int j = columnLabels().indexOf(col);
    QString s = (j == -1) ? QString() : row(i).at(j);
    return base::convert<T>(s);
}

#endif
