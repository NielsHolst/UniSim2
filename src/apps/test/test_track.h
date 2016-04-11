#ifndef TEST_TRACK_H
#define TEST_TRACK_H

#include <QFile>
#include <QSet>
#include "autotest.h"

namespace base {
class Box;
}

class TestTrack : public QObject
{
    Q_OBJECT
private slots:
    void testScalar();
    void testVector();
private:
    QFile file;
    void run(base::Box *simulation);
    void checkColumnNames();
    void openFile();
    void checkColumnNames(QSet<QString> expected);
    void checkColumnFormat(int expected);
    void checkColumnData(int expected);
};

DECLARE_TEST(TestTrack)

#endif
