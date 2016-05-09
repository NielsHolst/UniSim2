#include <iostream>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/test_num.h>
#include "output_file.h"
#include "test_population.h"

using namespace base;

void TestPopulation::testOk() {
    BoxBuilder builder;
    Box *sim(0);
    OutputFile output;
    QStringList cohortCols, expected;

    try {
        builder.
            box("Simulation").name("sim").
                port("steps").equals(8).
                box("boxes::Population").name("pop").
                    port("bufferSize").equals(3).
                    port("ageIncrement").equals(5).
                    port("initial").equals(10).
                    port("firstCohortGain").equals(4).
                    port("cohortsGain").imports("../gain[vector]").
                    box().name("gain").
                        newPort("vector").equals("(10.0 30.0 20.0)").
                    endbox().
                    box().name("loss").
                        newPort("vector").equals("(1.0 3.0 2.0)").
                    endbox().
                endbox().
                box("OutputR").
                    box("PageR").
                        box("PlotR").
                            port("ports").equals("(pop[cohorts] pop[ageIncrements] pop[lastCohortSpill] gain[vector])").
                        endbox().
                    endbox().
                endbox().
            endbox();

        sim = builder.content();
        sim->run();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));

        std::cout << qPrintable(output.columnLabels().join(", ")) << "\n"
                  << qPrintable(output.filePath()) << "\n";
        QCOMPARE(output.columnLabels().size(), 11);
        QCOMPARE(output.numRows(), 9);

        cohortCols << "cohorts_0" << "cohorts_1" << "cohorts_2";

        expected.clear();
        expected << "10" << "0" << "0";
        QCOMPARE(output.row(0, cohortCols), expected);
        QCOMPARE(output.cell(0, "lastCohortSpill"), QString("0"));

        expected.clear();
        expected << "20" << "4" << "20";
        QCOMPARE(output.row(1, cohortCols), expected);
        QCOMPARE(output.cell(1, "lastCohortSpill"), QString("30"));

        expected.clear();
        expected << "30" << "34" << "4";
        QCOMPARE(output.row(2, cohortCols), expected);
        QCOMPARE(output.cell(2, "lastCohortSpill"), QString("40"));

        expected.clear();
        expected << "4" << "64" << "24";
        QCOMPARE(output.row(3, cohortCols), expected);
        QCOMPARE(output.cell(3, "lastCohortSpill"), QString("40"));
    }

    delete sim;
}
