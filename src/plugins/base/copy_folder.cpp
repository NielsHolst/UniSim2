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

    void makeWritable(QString fileNamePath) {
        QFile::setPermissions(fileNamePath,
                              QFileDevice::ReadOwner|QFileDevice::WriteOwner|
                              QFileDevice::ReadUser|QFileDevice::WriteUser|
                              QFileDevice::ReadGroup|QFileDevice::WriteGroup|
                              QFileDevice::ReadOther|QFileDevice::WriteOther
                              );
    }

}

void copyFolder(QDir source, QDir destination) {
    if (destination.exists())
        renameFolder(destination);
    copyFolderHard(source, destination);
}

void renameFolder(QDir dir) {
    QString oldPath = dir.absolutePath();
    int i = 1;
    while (QDir(numbered(oldPath, i)).exists())
        ++i;
    QString newPath = numbered(oldPath, i);

    QString info = oldPath + " -> " + newPath;
    dialog().information("Renaming folder: " + info);

    bool ok = QDir().rename(oldPath, newPath);
    if (!ok)
        ThrowException("Cannot rename folder").value(info)
                      .hint("Close all programs then try again");
}

void copyFolderHard(QDir source, QDir destination) {
    // Create destination folder if needed; folders along the path are also created if needed
    if (!destination.exists()) {
        bool ok = QDir().mkpath(destination.absolutePath());
        if (!ok)
            ThrowException("Cannot create destination folder").value(destination.absolutePath())
                    .hint("Close all programs then try again");
    }

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
    // Avoid read-only permission
    makeWritable(destinationPath);
}

} //namespace
