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
    ~MainWindow();
private:
    // Date
    base::Dialog *_dialog;
    // Methods
    void restore();
};

#endif
