#include <QtWidgets>
#include <base/dialog.h>
#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget( _dialog = new base::Dialog(this) );
    resize(500, 300);
    setWindowTitle("Universal Simulator 2.0");
}

