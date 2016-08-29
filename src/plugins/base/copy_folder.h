#ifndef BASE_COPY_FOLDER_H
#define BASE_COPY_FOLDER_H
#include <QDir>
#include <QString>


namespace base {

void copyFolder(QDir source, QDir destination);
void copyFolderHard(QDir source, QDir destination);
void copyFile(QString sourcePath, QString destinationPath);
void backupFolder(QDir dir);

} //namespace

#endif
