#include <iostream>
#include <QSet>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/command.h>
#include <base/dialog.h>
#include <base/distribution.h>
#include <base/exception.h>
#include "exception_expectation.h"
#include "output_file.h"
#include "test_distribution.h"

using namespace base;
using namespace std;

void TestDistribution::testLoad() {
    bool excepted(false);
    int errors = dialog().errorCount();
    Box *sim;
    try {
        Command::submit(QStringList() << "load" << "distribution/monte_carlo.box", nullptr);
        sim = environment().root();
        sim->initializeFamily();
    }
    UNEXPECTED
    QCOMPARE(errors, dialog().errorCount());

    Distribution *uniform(nullptr), *normal(nullptr);
    try {
        uniform = sim->findOne<Distribution>("/sim/a/initial<Port>/*<Distribution>");
        normal  = sim->findOne<Distribution>("/sim/a/inflow<Port>/*<Distribution>");
    }
    UNEXPECTED

    double sample;
    QSet<double> samples;
    for (int i=0; i<30; ++i) {
        cout << i << " C\n";
        try {
            sample = uniform->draw();
        }
        UNEXPECTED
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

void TestDistribution::testBlindPort() {
    bool excepted(false);
    int errors = dialog().errorCount();
    Box *sim;
    try {
        Command::submit(QStringList() << "run" << "distribution/distribution-blind-port.box", nullptr);
        sim = environment().root();
        sim->initializeFamily();
    }
    UNEXPECTED
    QVERIFY(dialog().errorCount() <= errors);

    OutputFile file;
    std::cout << "*** " << qPrintable(file.filePath()) << " "
              << qPrintable(file.columnLabels().join(" ")) << "\n";

    QStringList values = file.column("f");
    QCOMPARE(values.size(), 20);
    QSet<QString> set;
    for (QString value : values.toVector())
        set << value;
    QCOMPARE(set.size(), 5);
}

void TestDistribution::testBuilder() {
    bool excepted(false);
    BoxBuilder builder;
    try {
        builder.
            box("Simulation").name("sim").
                box("SensitivityAnalysisSimple").endbox().
                box("Stage").name("a").
                    port("initial").equals(100).rnd("uniform").min(50).max(200).
                    port("inflow").equals(15).rnd("normal").mean(15).sd(2).
                endbox().
            endbox();
    }
    UNEXPECTED
    Box *sim = builder.content();

    Distribution *uniform(nullptr), *normal(nullptr);
    try {
        sim->initializeFamily();
        uniform = sim->findOne<Distribution>("/sim/a/initial<Port>/*<Distribution>");
        normal  = sim->findOne<Distribution>("/sim/a/inflow<Port>/*<Distribution>");
    }
    UNEXPECTED
    QVERIFY(uniform);
    QVERIFY(normal);

    QSet<double> samples;
    for (int i=0; i<30; ++i) {
        double sample = uniform->draw();
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
    bool excepted(false);
    Command::submit(QStringList() << "load" << "distribution/monte_carlo.box", nullptr);
    Box *sim = environment().root();
    Box *sa;
    try {
        sim->initializeFamily();
        sa = environment().root()->findOne<Box>("/sim/sa");
    }
    UNEXPECTED

    QCOMPARE(sa->port("inputsAnalysed")->value<int>(), 2);
    QCOMPARE(sa->port("inputsTotal")->value<int>(), 10); // Inputs to Stage; root and sa boxes are not counted
    delete sim;
}
