#ifndef TEST_BOX_H
#define TEST_BOX_H

#include "autotest.h"

class TestBox : public QObject
{
    Q_OBJECT
private slots:
    void test_create();
    void test_run();
};

DECLARE_TEST(TestBox)

#endif
