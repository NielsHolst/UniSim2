#ifndef TEST_TRACK_H
#define TEST_TRACK_H

#include "autotest.h"

class TestTrack : public QObject
{
    Q_OBJECT
private slots:
    void testDefault();
private:
    void save(QObject *parent);
    void checkColumnNames();
};

DECLARE_TEST(TestTrack)

#endif
