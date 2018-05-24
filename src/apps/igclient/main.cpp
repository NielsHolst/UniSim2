#define IGLIB_DLL dllimport

#include <iostream>

#include <QDir>
#include <fstream>

#include <iglib/iglib.h>
#include <base/environment.h>
#include <base/exception.h>

using namespace base;
using namespace ig;
using namespace std;

void writeBoxScript(const Query &q) {
    const char *boxScript = queryToString(q);
    environment().latestLoadArg("igclient.box");
    QFile file;
    environment().openOutputFile(file, ".box");
    file.write(qPrintable(boxScript));
    QString filePath = environment().outputFilePath(".box");
    std::cout << "Box script written to " << qPrintable(filePath) << "\n";
    environment().incrementFileCounter();
}

int main(int , char **)
{
//    QFile file(":/igclient/greenhouse-standard.box");
//    file.open(QIODevice::ReadOnly|QIODevice::Text);
//    string all = file.readAll().toStdString();
//    cout << all;

    try {
        Query q; // = randomQuery();
//        writeBoxScript(q);
        Response r = compute(q);
//        std::cout << responseToString(r) << "\n";
    }
    catch (Exception &ex) {
        std::cout << qPrintable(ex.what()) << "\n";
    }
    ig::release();
    return 0;
}
