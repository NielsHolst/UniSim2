#include <QCoreApplication>
#include <base/object_pool.h>
#include <base/organisation.h>
#include "main_window.h"

void myMsgHandler(QtMsgType, const QMessageLogContext &, const QString &msg)
{
    ThrowException(msg);
}


int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMsgHandler);
    QApplication app(argc, argv);
    app.setObjectName("application");
    new base::ObjectPool(&app);
    MainWindow window;
    window.show();
    return app.exec();
}
