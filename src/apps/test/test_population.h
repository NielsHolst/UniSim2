#ifndef TEST_POPULATION_H
#define TEST_POPULATION_H

#include "autotest.h"

class TestPopulation : public QObject
{
    Q_OBJECT
private slots:
    void testOk();
};

DECLARE_TEST(TestPopulation)

#endif
