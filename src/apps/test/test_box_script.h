#ifndef TEST_BOX_SCRIPT_H
#define TEST_BOX_SCRIPT_H
#include <QFile>
#include "autotest.h"

class TestBoxScript : public QObject
{
    Q_OBJECT
private slots:
    void testPlotRPortsAsVector();
    void testPlotRPortsAsScalar();
    void testPlotRPortsAsReference();

    void testLoadAbsolutePath();
private:
    QFile _file;
};

DECLARE_TEST(TestBoxScript)

#endif
