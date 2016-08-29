#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include "copy_folder.h"
#include "dialog.h"
#include "exception.h"

namespace base {

namespace {

    QString numbered(QString s, int i) {
        return s + "_" + QString::number(i);
    }
}

void copyFolder(QDir source, QDir destination) {
    if (destination.exists())
        backupFolder(destination);
    copyFolderHard(source, destination);
}

void copyFolderHard(QDir source, QDir destination) {
    // Create destination folder if needed
    bool ok = source.mkpath(destination.absolutePath());
    if (!ok)
        ThrowException("Cannot create destination folder").value(destination.absolutePath());

    // Copy folder
    QDirIterator it(source);
    while (it.hasNext()) {
        it.next();
        QFileInfo info = it.fileInfo();
        QString entryName = it.fileName(),
                sourcePath = it.filePath(),
                destinationPath = destination.absolutePath() + "/" + entryName;
        if (entryName == "." || entryName == "..")
            continue;
        if (info.isDir())
            copyFolderHard(QDir(sourcePath), QDir(destinationPath));
        else
            copyFile(sourcePath, destinationPath);
    }
}

void copyFile(QString sourcePath, QString destinationPath) {
    QFile source(sourcePath),
          destination(destinationPath);
    if (!source.exists())
        ThrowException("Source file does not exist").value(sourcePath);
    destination.remove();
    destination.close();
    bool fileCopied = source.copy(destinationPath);
    if (!fileCopied)
        ThrowException("Cannot copy file").value(sourcePath+" -> "+destinationPath);
}

void backupFolder(QDir dir) {
    QString path = dir.absolutePath();
    int i = 1;
    while (QDir(numbered(path, i)).exists())
        ++i;
    QString newPath = numbered(path, i);
    dialog().information("Backing up folder:\n" + path + " -> " + newPath + "...");
    copyFolderHard(dir, QDir(newPath));
}

} //namespace
