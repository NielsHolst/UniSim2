#include <iostream>
#include <memory>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include "QProcess"
#include <QProgressDialog>
#include <QString>
#include <QStringList>
#include <base/altova_xml.h>
#include <base/command.h>
#include <base/dialog_minimal.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/object_pool.h>
#include <base/organisation.h>
#include "main_window.h"

using namespace std;
using namespace base;

void myMsgHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    QMessageBox::warning(nullptr, "Unexpected error", msg);
}

int runWithDialog() {
    MainWindow window;
    window.show();
    return qApp->exec();
}

bool canWriteTo(QString fileName) {
    QFile f(fileName);
    bool ok = f.open(QIODevice::WriteOnly);
    f.close();
    return ok;
}

QString translateInputXml(QString inputFilePath) {
    AltovaXml altova;

    QFileInfo fi(inputFilePath);
    QString path = fi.absolutePath(),
            fileName = fi.fileName();

    if (fi.suffix().toLower() != "xml")
        ThrowException("Input must be an XML file").value(inputFilePath);

    QDir dir(path);
    if (!dir.exists())
        ThrowException("Input folder does not exist:\n" + path);
    dir.cdUp();
    dir.mkdir("temp");
    dir.cd("temp");
    if (!dir.exists())
        ThrowException("Temporary folder does not exist:\n" + dir.absolutePath());

    QString outputFilePath = dir.absoluteFilePath(fileName);
    altova.run(inputFilePath, outputFilePath);
    return outputFilePath;
}

QStringList extractArguments(int argc, char *argv[]) {
    QStringList list;
    for(int i = 1; i < argc; ++i) {
        list << argv[i];
    }
    if (argc != 3)
        ThrowException("Wrong number or arguments:\n"+list.join("\n"));
    return list;
}

void clearDestination(QString filePath) {
    if (QFile::exists(filePath)) {
        if (!QFile::remove(filePath))
            QMessageBox::warning(nullptr, "Error", "Cannot access output file:\n"+filePath);
    }
}

void copyFile(QString fromFilePath, QString toFilePath) {
    if (!QFile::copy(fromFilePath, toFilePath)) {
        QString message("Cannot copy output file\nFrom: %1\nTo: %2");
        ThrowException(message.arg(fromFilePath).arg(toFilePath));
    }
}

int runWithoutDialog(int argc, char *argv[]) {
    DialogMinimal *dialog = new DialogMinimal(qApp);
    environment().checkInstallation();
    QStringList args = extractArguments(argc, argv);

    QString inputFilePath = args.at(0),
        destinationFileName = args.at(1),
        destinationFilePath = QFileInfo(destinationFileName).absoluteFilePath();

    clearDestination(destinationFilePath);

    QString translatedFilePath = translateInputXml(inputFilePath);

    Command::submit(QStringList() << "set" << "folder" << "work" << "HOME", nullptr);
    Command::submit(QStringList() << "set" << "folder" << "input" << "input", nullptr);
    Command::submit(QStringList() << "set" << "folder" << "output" << "output", nullptr);

    dialog->resetErrorCount();
    Command::submit(QStringList() << "run" << translatedFilePath, nullptr);
    if (dialog->errorCount() > 0)
        ThrowException(dialog->getError());

    QString outputFilePath = environment().outputFilePath(".txt", -1 );
    copyFile(outputFilePath, destinationFilePath);

    return 0;
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMsgHandler);
    QApplication app(argc, argv);
    app.setObjectName("application");
    new base::ObjectPool(&app);

    bool hasArguments = (argc > 1);
    int result;
    try {
        result = hasArguments ? runWithoutDialog(argc, argv) : runWithDialog();
    }
    catch (Exception &ex){
        result = 1;
        QMessageBox::warning(nullptr, "Error", ex.what());
    }
    return result;
}
