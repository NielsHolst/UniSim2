#ifndef TEST_SAVANNA_H
#define TEST_SAVANNA_H

#include "autotest.h"

namespace base {
    class DialogStub;
}

class TestSavanna : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();
    void testDirectCreate();
    void testPort();
    void testPortDefaultValue();
    void testNoPort();
    void testNewPort();
    void testNewPortSum();
    void testImport();
private:
    base::DialogStub *dialogStub;
};

DECLARE_TEST(TestSavanna)

#endif
