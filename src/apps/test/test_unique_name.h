#ifndef TEST_UNIQUE_NAME_H
#define TEST_UNIQUE_NAME_H

#include "autotest.h"

class TestUniqueName : public QObject
{
    Q_OBJECT
private slots:
    void testMany();
    void testSandy();
};

DECLARE_TEST(TestUniqueName)

#endif
