#include <iostream>
#include <QCoreApplication>
#include <base/object_pool.h>
#include <base/organisation.h>
#include "autotest.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    new base::ObjectPool(&app);
    int result = AutoTest::run(argc, argv);
    QString msg = (result==0) ? "All tests passed" : "One or more tests failed";
    std::cout << "\n\n" << qPrintable(msg) << "\n";
    return result;
}
