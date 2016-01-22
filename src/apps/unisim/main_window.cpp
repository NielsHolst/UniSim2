#include <QtWidgets>
#include <console/text_edit.h>
#include "main_window.h"

//using console::TextEdit;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    _textEdit = new console::TextEdit;
    setCentralWidget(_textEdit);
    resize(500, 300);
    setWindowTitle("Universal Simulator 2.0");
}
