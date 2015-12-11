#include <iostream>
#include <QCoreApplication>
#include "autotest.h"


int main(int argc, char *argv[])
{
    int result = AutoTest::run(argc, argv);
    QString msg = (result==0) ? "All tests passed" : "One or more tests failed";
    std::cout << "\n\n" << qPrintable(msg) << "\n";
    return result;
}
