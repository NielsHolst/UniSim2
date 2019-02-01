#define IGLIB_DLL dllimport

#include <iostream>
#include <string>
#include <QApplication>
#include <QString>
#include <base/dialog_stub.h>
#include <iglib/iglib.h>
#include "query_reader_xml.h"

using namespace ig;
using namespace std;

const QString filePath = "/Users/au152367/Documents/QDev/UniSim2/input/projects/ig";


int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    new base::DialogStub(&app);
    int result = 0;
    QueryReaderXml reader;
    try {
//        Query q =
                reader.parse(filePath + "/ig-interface-test.xml");
        Response r; // = compute(q);

        if (r.hasError) {
            cout << "\n\nError in iglib:\n" << r.error << "\n";
        }
        else
            cout << responseToString(r);

        ig::release();
    }
    catch (QString msg) {
        cout << qPrintable(msg);
        result = 1;
    }
    cout << "\n\nDone!\n>";
    char s[8];
    cin.getline(s,7);
    return result;
}
