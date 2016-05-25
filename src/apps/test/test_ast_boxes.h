#ifndef TEST_AST_BOXES_H
#define TEST_AST_BOXES_H

#include "autotest.h"

class TestAstBoxes : public QObject
{
    Q_OBJECT
private slots:
    void testSimple();
private:
    QString filePath(QString fileName);
};

DECLARE_TEST(TestAstBoxes)

#endif
