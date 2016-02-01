#include <QSettings>
#include <QtWidgets>
#include <base/dialog.h>
#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget( _dialog = new base::Dialog(this) );
    setWindowTitle("Universal Simulator 2.0");
    restore();
}


void MainWindow::restore() {
    QSettings settings;
    QPoint pos(0,0);
    QSize siz(500,600);

    QPoint position = settings.value("main_window_geometry/position", pos).toPoint();
    QSize size = settings.value("main_window_geometry/size", siz).toSize();
    if (size.isNull()) size = siz;
    if (position.isNull()) position = pos;

    move(position);
    resize(size);
}

MainWindow::~MainWindow() {
    QSettings settings;
    settings.setValue("main_window_geometry/position", pos());
    settings.setValue("main_window_geometry/size", size());
}
