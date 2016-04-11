#ifndef TEST_WEATHER_H
#define TEST_WEATHER_H

#include "autotest.h"

class TestPortWeather : public QObject
{
    Q_OBJECT
private slots:
    void testOk();
};

DECLARE_TEST(TestPortWeather)

#endif
