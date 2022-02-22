#include <iostream>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/exception.h>
#include <base/reader_boxscript.h>
#include "exception_expectation.h"
#include "input_file_path.h"
#include "output_file.h"
#include "test_simulation.h"

void TestSimulation::testGrowth() {
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/simulation.box"));
        root = Box::root(builder.content());
    }
    UNEXPECTED_EXCEPTION;

    try {
        root->run();
    }
    UNEXPECTED_EXCEPTION;

    if (root->port("hasError")->value<bool>())
        QFAIL(qPrintable(root->port("errorMsg")->value<QString>()));

    OutputFile f;
    QCOMPARE(f.columnLabels(), QStringList()<<"iteration"<<"sim.step"<<"comp.step"<<"r"<<"density");
    QCOMPARE(f.numRows(), 4);
    QCOMPARE(f.cell<int>(3,"sim.step"), 3);
    QCOMPARE(f.cell<double>(3,"density"), 115.763);
}

void TestSimulation::testSteps() {
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/simulation_steps.box"));
        root = Box::root(builder.content());
        root->run();
    }
    UNEXPECTED_EXCEPTION;
    OutputFile f;
    QCOMPARE(f.numRows(), 4);
    QCOMPARE(f.cell<int>(0,"step"), 0);
    QCOMPARE(f.cell<int>(3,"step"), 3);
}

void TestSimulation::testCalendarSteps() {
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/simulation_calendar_steps.box"));
        root = Box::root(builder.content());
        root->run();
    }
    UNEXPECTED_EXCEPTION;
    OutputFile f;
    QCOMPARE(f.numRows(), 4);
    QCOMPARE(f.cell<QDate>(0,"date"), QDate(2022,2,24));
    QCOMPARE(f.cell<QDate>(3,"date"), QDate(2022,2,27));
}

void TestSimulation::testCalendarPeriodDateDayStep() {
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/simulation_calendar_period_date_day_step.box"));
        root = Box::root(builder.content());
        root->run();
    }
    UNEXPECTED_EXCEPTION;
    OutputFile f;
    QCOMPARE(f.numRows(), 5);
    QCOMPARE(f.cell<QDate>(0,"date"), QDate(2022,2,24));
    QCOMPARE(f.cell<QDate>(4,"date"), QDate(2022,2,28));
}

void TestSimulation::testCalendarPeriodDateHourStep() {
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/simulation_calendar_period_date_hour_step.box"));
        root = Box::root(builder.content());
        root->run();
    }
    UNEXPECTED_EXCEPTION;
    OutputFile f;
    QCOMPARE(f.numRows(), 17);
    QCOMPARE(f.cell<QDate>( 0,"date"), QDate(2022,2,24));
    QCOMPARE(f.cell<QDate>(16,"date"), QDate(2022,2,28));
    QCOMPARE(f.cell<QTime>( 0,"time"), QTime( 0,0,0));
    QCOMPARE(f.cell<QTime>(15,"time"), QTime(18,0,0));
    QCOMPARE(f.cell<QTime>(16,"time"), QTime( 0,0,0));
}
