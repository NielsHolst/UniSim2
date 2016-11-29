#include <iostream>
#include <QApplication>
//#include <base/dialog_stub.h>
//#include <base/object_pool.h>
#include "unisimlib_test.h"

using std::cout;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    std::cout << "Hello A\n";
    UniSim::Input input{27,10};
    UniSim::Output output = UniSim::run1(input);

    for (int x : output.sequence)
        cout << x << " ";
    cout << "\n";

    UniSim::close();
    cout << "Hello Z\n";

    return 0;
}
