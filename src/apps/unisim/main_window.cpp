#include <QApplication>
#include <QDesktopWidget>
#include <QSettings>
#include <QtWidgets>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/version.h>
#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    _dialog = new base::Dialog(this);
    setCentralWidget(_dialog->textEdit());
    setWindowTitle("Universal Simulator " + base::version());
    restore();
    base::environment().checkInstallation();
}


void MainWindow::restore() {
    QSettings settings;
    restoreGeometry(settings.value("main_window/geometry").toByteArray());
    restoreState(settings.value("main_window/state").toByteArray());

//    // Get screen dimensions
//    QDesktopWidget *desktop = QApplication::desktop();
//    QRect screen = desktop->availableGeometry(this);
//    int W = screen.width(),
//        H = screen.height();
//    // Get latest screen geometry
//    QSettings settings;
//    QPoint position = settings.value("main_window_geometry/position", QPoint(W/4, H/4)).toPoint();
//    QSize size = settings.value("main_window_geometry/size", QSize(W/2, H/2)).toSize();
//    // Adjust position if outside
//    int x = position.x(),
//        y = position.y();
//    if (x<0 || x>W)
//        x = W/4;
//    if (y<0 || y>H)
//        y = H/4;
//    // Adjust size if window is too big
//    int width  = size.width(),
//        height = size.height(),
//        maxWidth  = W - x,
//        maxHeight = H - y;
//    if (width > maxWidth)
//        width = maxWidth;
//    if (height > maxHeight)
//        height = maxHeight;
//    // Restore window geometry
//    resize(QSize(width, height));
//    move(QPoint(x,y));
}

MainWindow::~MainWindow() {
    QSettings settings;
//    settings.setValue("main_window_geometry/position", pos());
//    settings.setValue("main_window_geometry/size", size());
    settings.setValue("main_window/geometry", saveGeometry());
    settings.setValue("main_window/state", saveState());
}
