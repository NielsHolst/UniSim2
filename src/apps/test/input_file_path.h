#ifndef INPUT_FILE_PATH_H
#define INPUT_FILE_PATH_H
#include <QFile>
#include <base/environment.h>
#include <base/exception.h>

inline QString inputFilePath(QString fileName) {
    return base::environment().filePath(base::Environment::Input, fileName);
}

inline void openInputFile(QFile &file, QString fileName) {
    file.setFileName(inputFilePath(fileName));
    bool ok = file.open(QIODevice::Text|QIODevice::ReadOnly);
    if (!ok)
        ThrowException("Cannot open file").value(inputFilePath(fileName));
}


#endif // INPUT_FILE_PATH_H

