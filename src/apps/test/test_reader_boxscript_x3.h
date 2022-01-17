#ifndef TEST_READER_BOXSCRIPT_X3_H
#define TEST_READER_BOXSCRIPT_X3_H
#include "autotest.h"

class TestReaderBoxScriptX3 : public QObject
{
    Q_OBJECT
private slots:
    void testAuxAllTypes();
    void testConstructAmend();
    void testInitialize();
    void testInitializePortMissing();
};

DECLARE_TEST(TestReaderBoxScriptX3)

#endif
