#include <QSet>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/command.h>
#include <base/dialog.h>
#include <base/distribution.h>
#include <base/exception.h>
#include "test_distribution.h"

using namespace base;

void TestDistribution::testLoad() {
    int errors = dialog().errorCount();
    Box *sim;
    try {
        Command::submit(QStringList() << "load" << "distribution/monte_carlo.box", 0);
        sim = environment().root();
        sim->initializeFamily();
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
    QCOMPARE(errors, dialog().errorCount());

    Distribution *even(0), *normal(0);
    try {
        Box *a = sim->findOne<Box>("/sim/a");
        even = a->findChild<Distribution*>("uniform", Qt::FindDirectChildrenOnly);
        normal = a->findChild<Distribution*>("normal", Qt::FindDirectChildrenOnly);
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
    QVERIFY(even);
    QVERIFY(normal);

    double sample;
    QSet<double> samples;
    for (int i=0; i<30; ++i) {
        try {
            sample = even->draw();
        }
        catch(Exception &ex) {
            QFAIL(qPrintable(ex.what()));
        }
        samples << sample;
        QVERIFY(sample>=50 && sample<200);
    }
    QCOMPARE(samples.size(), 30);

    samples.clear();
    for (int i=0; i<30; ++i) {
        try {
            sample = normal->draw();
        }
        catch(Exception &ex) {
            QFAIL(qPrintable(ex.what()));
        }
        samples << sample;
        // R qnorm(0.01, 15, 2) = 10.3473
        // R qnorm(0.99, 15, 2) = 19.6527
        QVERIFY2(sample>=10.3473 && sample<19.6527, qPrintable(QString::number(sample)));
    }
    QCOMPARE(samples.size(), 30);
    delete sim;
}

void TestDistribution::testBuilder() {
    BoxBuilder builder;
    try {
        builder.
            box("Simulation").name("sim").
                box("SensitivityAnalysis").endbox().
                box("Stage").name("a").
                    port("initial").equals(100).rnd("uniform").min(50).max(200).
                    port("inflow").equals(15).rnd("normal").mean(15).sd(2).
                endbox().
            endbox();
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
    Box *sim = builder.content();

    Distribution *even(0), *normal(0);
    try {
        sim->initializeFamily();
        Box *a = sim->findOne<Box>("/sim/a");
        even = a->findChild<Distribution*>("uniform", Qt::FindDirectChildrenOnly);
        normal = a->findChild<Distribution*>("normal", Qt::FindDirectChildrenOnly);
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
    QVERIFY(even);
    QVERIFY(normal);

    QSet<double> samples;
    for (int i=0; i<30; ++i) {
        double sample = even->draw();
        samples << sample;
        QVERIFY(sample>=50 && sample<200);
    }
    QCOMPARE(samples.size(), 30);

    samples.clear();
    for (int i=0; i<30; ++i) {
        double sample = normal->draw();
        samples << sample;
        // R qnorm(0.01, 15, 2) = 10.3473
        // R qnorm(0.99, 15, 2) = 19.6527
        QVERIFY2(sample>=10.3473 && sample<19.6527, qPrintable(QString::number(sample)));
    }
    QCOMPARE(samples.size(), 30);
    delete sim;
}

void TestDistribution::testMonteCarlo() {
    Command::submit(QStringList() << "load" << "distribution/monte_carlo.box", 0);
    Box *sim = environment().root();
    Box *sa;
    try {
        sim->initializeFamily();
        sa = environment().root()->findOne<Box>("/sim/sa");
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
    QCOMPARE(sa->port("inputsAnalysed")->value<int>(), 2);
    QCOMPARE(sa->port("inputsTotal")->value<int>(), 6+4); // StageBase + Stage
    delete sim;
}
