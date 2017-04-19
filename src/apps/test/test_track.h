#ifndef TEST_TRACK_H
#define TEST_TRACK_H

#include <QFile>
#include <QVector>
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
    void testTrackPtr();
    void testParseOk();
    void testParseError();
    void testNoFilter();
    void testFilter();
private:
    QFile file;
    void run(base::Box *simulation);
    void checkColumnNames();
    void openFile();
    void checkColumnNames(QSet<QString> expected);
    void checkColumnFormat(int expected);
    void checkColumnData(int expected);
    base::Box* buildJump(int start, int length);
    QVector<int> runJump(base::Box *sim);
};

DECLARE_TEST(TestTrack)

#endif
