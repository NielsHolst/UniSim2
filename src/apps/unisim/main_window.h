#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

namespace base {
    class Dialog;
}

class MainWindow : public QMainWindow
{

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    // Date
    base::Dialog *_dialog;
    // Methods
    void restore();
};

#endif
