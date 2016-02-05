#include <iostream>
#include <QCoreApplication>
#include <base/organisation.h>
#include "autotest.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    int result = AutoTest::run(argc, argv);
    QString msg = (result==0) ? "All tests passed" : "One or more tests failed";
    std::cout << "\n\n" << qPrintable(msg) << "\n";
    return result;
}
