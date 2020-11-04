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
    if (settings.contains("main_window/geometry"))
        restoreGeometry(settings.value("main_window/geometry").toByteArray());
    else {
        // Get screen dimensions
        QDesktopWidget *desktop = QApplication::desktop();
        QRect screen = desktop->availableGeometry(this);
        int W = screen.width(),
            H = screen.height();
        // Position within screen
        QPoint position = QPoint(W/4, H/4);
        QSize size = QSize(W/2, H/2);
        move(position);
        resize(size);
    }
    if (settings.contains("main_window/state"))
        restoreState(settings.value("main_window/state").toByteArray());
}

MainWindow::~MainWindow() {
    QSettings settings;
    settings.setValue("main_window/geometry", saveGeometry());
    settings.setValue("main_window/state", saveState());
}
