#define IGLIB_DLL dllimport

#include <iostream>
#include <string>
#include <QFileInfo>
#include <QString>
#include <base/box.h>
#include <base/command.h>
#include <base/environment.h>
#include <base/exception.h>
#include <iglib/iglib.h>
#include "query_reader_json.h"

using namespace ig;
using namespace std;

const QString filePath = "D:/Documents/QDev/UniSim2/input/projects/ig/"
                         "2022-03-25-light-error-amended.json";
//                       "2020-10-14-igdata-no-light.json";
//                       "2020-10-14-igdata.json";
//                       "UnisimInput 2020-02.json";
using namespace base;

int main(int, char **)
{
    int result = 0;
    QueryReaderJson reader;
    try {
        Response r;
        if (QFileInfo::exists(filePath)) {
            Query q = reader.parse(filePath);
            r = compute(q);
        }
        else {
            r = blankResponse();
        }
        cout << "\nRESPONSE:\n" << responseToString(r) << "\n";
    }
    catch (const Exception &ex) {
        cout << qPrintable(ex.what());
        result = 1;
    }
    catch (const std::exception &ex) {
        cout << ex.what();
        result = 2;
    }


    return result;
}
