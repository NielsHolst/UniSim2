#define IGLIB_DLL dllimport

#include <iostream>
#include <string>
#include <QString>
#include <base/exception.h>
#include <iglib/iglib.h>
#include "query_reader_json.h"

using namespace ig;
using namespace std;

const QString filePath = "/Users/au152367/Documents/QDev/UniSim2/input/projects/ig/"
                         "UnisimInput_2019-09-10_2019-09-11.json";


int main(int, char **)
{
    int result = 0;
    QueryReaderJson reader;
    try {
        Query q = reader.parse(filePath);
        Response r = compute(q);
        cout << "\nRESPONSE:\n" << responseToString(r) << "\n";
    }
    catch (const base::Exception &ex) {
        cout << qPrintable(ex.what());
        result = 1;
    }
    catch (const std::exception &ex) {
        cout << ex.what();
        result = 2;
    }
    cout << "\n\nDone!\nHit <Enter>";
    char s[8];
    cin.getline(s,7);
    return result;
}
