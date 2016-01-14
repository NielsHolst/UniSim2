#ifndef TEST_SAVANNA_H
#define TEST_SAVANNA_H

#include "autotest.h"

class TestSavanna : public QObject
{
    Q_OBJECT
private slots:
    void testDirectCreate();
    void testPort();
    void testImportOnce();
};

DECLARE_TEST(TestSavanna)

#endif
