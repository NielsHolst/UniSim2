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
    help("creates an output text file");
}

void OutputText::initialize() {
    Track::initializeAll();
}

void OutputText::reset() {
    Track::resetAll();
}

void OutputText::update() {
    Track::updateAll();
}

void OutputText::cleanup() {
    Track::cleanupAll();
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

    // Write column labels
    QStringList list;
    for (Track *track : Track::all())
        list << track->uniqueNameExpanded();
    _stream << list.join("\t") << "\n";

    // Write column format
    list.clear();
    for (Track *track : Track::all()) {
        for (int i = 0; i < track->port()->valueSize(); ++i)
           list << track->port()->format();
    }
    _stream << list.join("\t") << "\n";

    // Write column values as text
    Track *last = Track::all().last();
    int nrow = Track::bufferSize();
    for (int row = 0; row < nrow; ++row) {
        for (Track *track : Track::all()) {
            _stream << track->toString(row);
            if (track != last)
                _stream << "\t";
        }
        if (row < nrow-1)
            _stream << "\n";
    }
}

}
