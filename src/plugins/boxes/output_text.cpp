#include <iostream>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/phys_math.h>
#include <base/port.h>
#include <base/publish.h>
#include <base/track.h>
#include "output_r.h"
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

void OutputText::initialize() {
    // Allocate track running
    _tracksRunningSum.resize(Track::all().size());
    _tracksSum.resize(Track::all().size());
    _tracksMin.resize(Track::all().size());
    _tracksMax.resize(Track::all().size());

    _isCondensed = false;
    for (Track *track : Track::all()) {
        if (track->isFiltered()) {
            _isCondensed = true;
            break;
        }
    }

    // Open output file and write column headings and formats
    openFileStream();
    writeColumnLabels();
    if (!skipFormats)
        writeColumnFormats();

    // If output is for R then add R code to skip format
    if (skipFormats) {
        OutputR *outputR = findMaybeOne<OutputR>("*");
        if (outputR)
            outputR->addRCode("output_skip_formats = TRUE");
    }
}

void OutputText::reset() {
    _tracksRunningSum.fill(0.);
    _tracksSum.fill(0.);
    _tracksMin.fill( phys_math::infinity());
    _tracksMax.fill(-phys_math::infinity());
    if (averageN < 1)
        ThrowException("'averagingN' must be larger then zero").value(averageN);
    _rowCount = 0;
    processValues();
}

void OutputText::update() {
    processValues();
}

void OutputText::debrief() {
    dialog().information("Data frame written to '" + environment().latestOutputFilePath("txt") + "'");
    _file.close();
}

void OutputText::openFileStream() {
    environment().openOutputFile(_file, "txt");
    _stream.setDevice(&_file);
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

inline isNumber(Track *track) {
    QString format = track->port()->format();
    return format.isEmpty() || format=="NA";
}

void OutputText::processValues() {
    // To collect values to be written
    QStringList values;

    // Average rows
    if (averageN > 1) {
        // We have summed up averageN rows: list the average or current value
        if (_rowCount > skipInitialRows && (_rowCount-skipInitialRows) % averageN == 0) {
            int i = 0;
            for (Track *track : Track::all()) {
                if (isNumber(track))
                    values << QString::number(_tracksRunningSum.at(i)/averageN);
                else
                    values <<  track->port()->value<QString>();
                i++;
            }
            _tracksRunningSum.fill(0.);
        }
        // Sum up rows when passed skipping
        if (_rowCount >= skipInitialRows) {
            int i = 0;
            for (Track *track : Track::all()) {
                if (isNumber(track))
                    _tracksRunningSum[i] += track->port()->value<double>();
                i++;
            }
        }
    }
    // No averaging: just list the values if passed skipping
    else if (_rowCount >= skipInitialRows) {
        if (_isCondensed) {
            int i = 0;
            for (Track *track : Track::all()) {
                if (isNumber(track)) {
                    double value = track->port()->value<double>();
                    _tracksSum[i] += value;
                    if (value < _tracksMin.at(i)) _tracksMin[i] = value;
                    if (value > _tracksMin.at(i)) _tracksMax[i] = value;
                }
                i++;
            }
        }
        else {
            for (Track *track : Track::all())
                values <<  track->port()->value<QString>();
        }
    }
    // Write the value list
    if (!values.isEmpty())
        _stream << values.join("\t") << "\n";
    ++_rowCount;
}

void OutputText::cleanup() {
    if (!_isCondensed)
        return;
    QStringList values;

    int i = 0;
    for (Track *track : Track::all()) {
        if (isNumber(track)) {
            if (track->filter() == PortFilter::Sum)
                values << QString::number(_tracksSum.at(i));
            else if (track->filter() == PortFilter::Mean)
                values << QString::number(_tracksSum.at(i)/_rowCount);
            else if (track->filter() == PortFilter::Min)
                values << QString::number(_tracksMin.at(i));
            else if (track->filter() == PortFilter::Max)
                values << QString::number(_tracksMax.at(i));
            else
                values <<  track->port()->value<QString>();
        }
        else {
            values <<  track->port()->value<QString>();
        }
        i++;
    }
    _stream << values.join("\t") << "\n";
}

}
