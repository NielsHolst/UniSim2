#include <iostream>
#include <QApplication>
#include <base/exception.h>
#include "unisimlib_test.h"

using std::cout;

void run1(QString id, int begin, int n) {
    cout << "\nBegin " << qPrintable(id) << "\n";

    UniSim::Input input{begin, n};
    UniSim::Output output = UniSim::run1(input);

    for (int i = 0; i < output.size; ++i)
        cout << output.sequence[i] << " ";

    cout << "\n";
    cout << "End " << qPrintable(id) << "\n";
}

void run2(QString id, int begin, int n) {
    cout << "\nBegin " << qPrintable(id) << "\n";

    UniSim::Input input{begin, n};
    UniSim::Output output;
    UniSim::run2(&input, &output);

    for (int i = 0; i < output.size; ++i)
        cout << output.sequence[i] << " ";

    cout << "\n";
    cout << "End " << qPrintable(id) << "\n";
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    std::cout << "Hello A\n";

    try {
        run1("first", 27, 10);
        run1("second", 10, 5);

        run2("first again", 27, 12);
        run2("second again", 10, 7);
    }
    catch(base::Exception &ex) {
        cout << "\nUnexpected exception: " << qPrintable(ex.what()) << "\n";
    }

    cout << "Hello Z\n";

    return 0;
}
