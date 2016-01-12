#ifndef TEST_PATH_H
#define TEST_PATH_H

#include "autotest.h"

namespace boxes {
    class  Box;
}


class TestPath : public QObject
{
    Q_OBJECT
private:
    void setContext(QString path = "");
    void compareVectors(QVector<QObject*> v1, QVector<QObject*> v2, int size);
    QObject *_root, *_context;

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testValidateName();
    void testValidateStep();

    void testNormalise();
    void testNormaliseInvalid();

    void testSetContext();
    void testSelf();
    void testChildren();
    void testParent();
    void testNearest();
    void testDescendants();
    void testAncestors();
    void testAllSiblings();
    void testOtherSiblings();
    void testPreceedingSibling();
    void testFollowingSibling();

    void testGlobal();
    void testNumberOfMatches();

    void testResolveInvalid();
};

DECLARE_TEST(TestPath)

#endif