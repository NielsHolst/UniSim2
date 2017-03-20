#ifndef TEST_DISTRIBUTION_H
#define TEST_DISTRIBUTION_H
#include "autotest.h"

class TestDistribution : public QObject
{
    Q_OBJECT
private slots:
    void testLoad();
    void testBuilder();
};

DECLARE_TEST(TestDistribution)

#endif
