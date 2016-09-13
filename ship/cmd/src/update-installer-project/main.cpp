#include <exception>
#include <iostream>
#include <QCoreApplication>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include "../common/common.h"

using namespace std;

bool argsOk(int argc) {
    if (argc == 4)
        return true;
    cout << "Three arguments are needed";
    return false;
}

bool updateVersion(char *argv[]) {
    QFile file(pathProject() + "/ship/unisim2_install_builder.xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        cout << "Could not open file " << qPrintable(file.fileName());
        return false;
    }
    QString s = file.readAll();
    if (s.isEmpty()) {
        cout << "Nothing read from " << qPrintable(file.fileName());
        return false;
    }

    int begin = s.indexOf("<version>"),
        end = s.indexOf("</version>");
    if (begin==-1 || end==-1) {
        cout << "Missing <version> element";
        return false;
    }

    QString result =
            s.left(begin) +
            "<version>" +
            argv[1] + "." + argv[2] + "." + argv[3] +
            s.mid(end);

    file.close();
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        cout << "Could not open file " << qPrintable(file.fileName());
        return false;
    }
    file.write(qPrintable(result));
    return true;
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    if (!argsOk(argc) || !updateVersion(argv))
        return 1;
    return 0;
}

