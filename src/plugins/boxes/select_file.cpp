#include <QDir>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "select_file.h"


using namespace base;

namespace boxes {

PUBLISH(SelectFile)

SelectFile::SelectFile(QString name, QObject *parent)
    : Box(name, parent)
{
    help("selects a file from a folder");
    Input(folder).help("Relative path to folder");
    Input(filter).equals("*.txt").help("File filter");
    Input(selectFileNumber).equals(1).help("Which file to pick");
    Input(fileNumberOffset).equals(1).help("Offset for fileNumber, typically 0 or 1");
    Input(showFileNames).equals(true).help("Show set of file names?").unit("bool");
    Output(numFiles).noReset().help("Number of matching files");
    Output(fileName).help("Name of selected file");
    Output(fileNamePath).help("Name of selected file with absolute path");
}


void SelectFile::amend() {
    QDir dir = environment().currentBoxScriptFolder();
    dir.cd(folder);
    _files = dir.entryList(QStringList() << filter, QDir::Files);
    numFiles = _files.size();
    reset(1);
}

void SelectFile::initialize() {
    if (showFileNames) {
        dialog().information(QString("Select among %1 files (%2) from %3")
                             .arg(_files.size()).arg(filter).arg(folder));
        for (QString fileName : _files.toVector()) {
            dialog().information(fileName);
        }
    }
}

void SelectFile::reset() {
    reset(selectFileNumber);
}

void SelectFile::reset(int fileNumber) {
    int i = fileNumber - fileNumberOffset;
    if (i > numFiles)
        ThrowException("No files found").value(folder).value2(filter).context(this);
    fileName = _files.at(i);
    fileNamePath = folder + "/" + fileName;
}

}
