#ifndef TEST_CONDITIONAL_H
#define TEST_CONDITIONAL_H

#include "autotest.h"

class TestConditional : public QObject
{
    Q_OBJECT
private slots:
    void testSimple();
};

DECLARE_TEST(TestConditional)

#endif
