#ifndef TEST_CONVERT_H
#define TEST_CONVERT_H

#include "autotest.h"

class TestConvert : public QObject
{
    Q_OBJECT
private slots:
    void testStringListFromString();
    void testIntFromStringList();
    void testDate();
    void testTimeToDouble();
};

DECLARE_TEST(TestConvert)

#endif
