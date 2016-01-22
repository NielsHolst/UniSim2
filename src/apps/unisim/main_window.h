#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace console {
    class TextEdit;
}

class TextEdit;

class MainWindow : public QMainWindow
{

public:
    MainWindow(QWidget *parent = 0);
private:
    console::TextEdit *_textEdit;
};

#endif
