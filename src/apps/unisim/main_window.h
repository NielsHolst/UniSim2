#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace base {
    class Dialog;
}

class MainWindow : public QMainWindow
{

public:
    MainWindow(QWidget *parent = 0);
private:
    base::Dialog *_dialog;
};

#endif
