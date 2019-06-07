#define IGLIB_DLL dllimport

#include <iostream>
#include <string>
#include <QString>
#include <base/exception.h>
#include <iglib/iglib.h>
#include "query_reader_json.h"

using namespace ig;
using namespace std;

const QString filePath = "/Users/au152367/Documents/QDev/UniSim2/input/projects/ig/UnisimInputData-1-day.json";


int main(int, char **)
{
    int result = 0;
    QueryReaderJson reader;
    try {
        Query q = reader.parse(filePath);
        Response r = compute(q);
        if (r.hasError) {
            cout << "\n\nError in iglib:\n" << r.error << "\n";
        }
        else
            cout << responseToString(r);

        r = testConstant(q);
        cout << "testConstant\n" << responseToString(r);

        q.timeStamp.dayOfYear = 10;
        q.timeStamp.timeOfDay = 7;
        q.timeStamp.timeZone = 3;
        r = testMultiplum(q);
        cout << "testMultiplum\n" << responseToString(r);
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
