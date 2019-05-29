#ifndef SELECT_FILE_H
#define SELECT_FILE_H
#include <QStringList>
#include <base/box.h>

namespace boxes {

class SelectFile : public base::Box
{
public:
    SelectFile(QString name, QObject *parent);
    void amend();
    void reset();
private:
    // Input
    QString folder, filter;
    int selectFileNumber, fileNumberOffset;
    // Output
    int numFiles;
    QString fileName, fileNamePath;
    // Data
    QStringList _files;
    // Methods
    void reset(int fileNumber);
};

}

#endif
