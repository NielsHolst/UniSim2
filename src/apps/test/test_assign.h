#ifndef TEST_ASSIGN_H
#define TEST_ASSIGN_H

#include "autotest.h"

class TestAssign : public QObject
{
    Q_OBJECT
private slots:
    void testToCharFromBool();
    void testToCharFromChar();
    void testToCharFromBoolVector();

    void testToCharVectorFromBoolVector();
    void testToCharVectorFromCharVector();
    void testToCharVectorFromBool();

    void testToStringFromInt();
    void testToStringFromIntVector();
    void testToStringFromDate();

    void testToIntFromDate();
    void testToIntFromTime();
    void testToDoubleFromTime();
    void testToDoubleFromDateTime();

};

DECLARE_TEST(TestAssign)

#endif
