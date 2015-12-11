#ifndef TEST_SAVANNA_H
#define TEST_SAVANNA_H

#include "autotest.h"

class TestSavanna : public QObject
{
    Q_OBJECT
private slots:
    void test_direct_create();
};

DECLARE_TEST(TestSavanna)

#endif
