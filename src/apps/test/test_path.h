#ifndef TEST_PATH_H
#define TEST_PATH_H

#include "autotest.h"

namespace base {
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
    void testRobustness();

    void testSetContext();
    void testSelf();
    void testChildren();
    void testParent();
    void testParentInPath();
    void testNearest();
    void testDescendants();
    void testAncestors();
    void testAllSiblings();
    void testOtherSiblings();
    void testPreceedingSibling();
    void testFollowingSibling();

    void testPathList();
    void testGlobal();
    void testRoot();
    void testBarePort();
    void testNumberOfMatches();

    void testEmpty();
    void testResolveInvalid();
    void testIndirections();

    void testDistribution();

    // Test from box scripts
    void testDistributionFromScript();
    void testCombination();
};

DECLARE_TEST(TestPath)

#endif
