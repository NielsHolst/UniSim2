#include <iostream>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/port.h>
#include <base/publish.h>
#include <base/track.h>
#include "output_text.h"

using namespace base;

namespace boxes {

PUBLISH(OutputText)

OutputText::OutputText(QString name, QObject *parent)
    : OutputPorts(name, parent)
{
    Class(OutputText);
    Input(skipFormats).equals(false).help("Skip line with column formats?");
    Input(skipInitialRows).equals(0).help("Skip this number of data frame rows");
    Input(averageN).equals(1).help("If N>1 then rows will be averaged for every N rows");
    help("creates an output text file");
}

void OutputText::debrief() {
    openFileStream(".txt");
    writeDataFrame();
    dialog().information("Data frame written to '" + environment().latestOutputFilePath("txt") + "'");
    _file.close();
}

void OutputText::openFileStream(QString extension) {
    environment().openOutputFile(_file, extension);
    _stream.setDevice(&_file);
}

void OutputText::writeDataFrame() {
    // Check ports
    if (Track::all().isEmpty())
        ThrowException("No ports are being tracked");
    if (averageN < 1)
        ThrowException("averagingN must be larger then zero").value(averageN);
    checkBufferSizes();
    writeColumnLabels();
    if (!skipFormats)
        writeColumnFormats();
    writeColumnValues();

}

void OutputText::checkBufferSizes() {
    int n = Track::all().at(0)->buffer()->size();
    for (Track *track : Track::all()) {
        int m = track->buffer()->size();
        if (m != n) {
            std::cout << "OutputText::writeDataFrame:\n";
            std::cout << qPrintable(Track::dumpOrders());
            std::cout << qPrintable(Track::dumpTracks());
            ThrowException("Unexpected error: buffer sizes do not match")
                    .value(n).value2(m).context(this);
        }
    }
}

void OutputText::writeColumnLabels() {
    QStringList list;
    for (Track *track : Track::all())
        list << track->uniqueNameExpanded();
    _stream << list.join("\t") << "\n";
}

void OutputText::writeColumnFormats() {
    QStringList list;
    list.clear();
    for (Track *track : Track::all()) {
        for (int i = 0; i < track->port()->valueSize(); ++i)
           list << track->port()->format();
    }
    _stream << list.join("\t") << "\n";
}

void OutputText::writeColumnValues() {
    Track *last = Track::all().last();
    int nrow = last->buffer()->size();
    for (int row = skipInitialRows; row < nrow; ++row) {
        QStringList values;
        if (averageN == 1)
            values = getRowValues(row);
        else if (row > skipInitialRows && (row-skipInitialRows) % averageN == 0)
            values = getAverageRowValues(row - averageN);
        if (!values.isEmpty())
            _stream << values.join("\t") << "\n";
    }
}

QStringList OutputText::getRowValues(int row) {
    QStringList values;
    for (Track *track : Track::all())
        values << track->toString(row);
    return values;
}

QStringList OutputText::getAverageRowValues(int firstRow) {
    QStringList values;
    for (Track *track : Track::all())
        values << getAverageTrackValue(track, firstRow);
    return values;
}

QString OutputText::getAverageTrackValue(Track *track, int firstRow) {
    QString result,
            format = track->port()->format();
    bool makeAverage = format.isEmpty() || format=="NA";
    if (makeAverage) {
        double sum = 0;
        for (int i=0; i<averageN; ++i) {
            QString value = track->toString(firstRow+i);
            sum += convert<double>(value);
        }
        result = convert<QString>(sum/averageN);
    }
    else {
        result = track->toString(firstRow + averageN/2);
    }
    return result;
}

}
