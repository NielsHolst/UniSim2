#ifndef TEST_HISTORY_H
#define TEST_HISTORY_H

#include "autotest.h"

class TestHistory : public QObject
{
    Q_OBJECT
private slots:
    void testAddOne();
    void testAddMany();
};

DECLARE_TEST(TestHistory)

#endif
