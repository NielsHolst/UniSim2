/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QDir>
#include <QMap>
#include <QMapIterator>
#include <QSet>
#include <QTextStream>
#include <QVector>
#include <base/box_builder.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/mega_factory.h>
#include <base/path.h>
#include <base/port.h>
#include <base/publish.h>
#include "output_r.h"
#include "page_r.h"

using namespace base;

namespace boxes {

PUBLISH(OutputR)

OutputR::OutputR(QString name, Box *parent)
    : Box(name, parent)
{
    help("creates output and scripts for R");
    sideEffects("writes an R script to the output folder\ncopies an R script to the clipboard");
    Input(clearMemory).equals(false).help("Clear R memory?");
    Input(clearPlots).equals(false).help("Clear previous plots before showing new plots in R?");
    Input(showPlots).equals(true).help("Show plots in R?");
    Input(showLines).equals(0).help("Number of lines to show at the prompt");
    Input(popUp).equals(false).help("Show plots in pop-up windows?");
    Input(width).equals(7).help("Width of pop-up windows (only used if popUp is set)");
    Input(height).equals(7).help("Height of pop-up windows (only used if popUp is set)");
    Input(plotAsList).equals(false).help("Put plots into an R list?");
    Input(saveDataFrame).equals(false).help("Save output as R data frame?");
    Input(skipFormats).imports("OutputWriter::*[skipFormats]");
    Input(scripts).help("R scripts to be run at the end");
    Input(plotTypes).computes("PlotR::*[type]");
    Output(ports).noClear().computes("./PageR::*[xAxis] | ./*/PlotR::*[ports]").help("Path to all ports used by my pages and plots");
    Output(numPages).noClear().help("Number of pages in this output");
}

void OutputR::amend() {
    // If it does not exist, create an OutputWriter on the root
    if (!findMaybeOne<Box*>("OutputWriter::*")) {
        BoxBuilder(Box::root()).
        box("OutputWriter").name("outputWriter").endbox();
        findOne<Box*>("/OutputWriter::outputWriter")->amendFamily();
    }
}

void OutputR::initialize() {
    // Check that only one OutputR objects exists
    if (findMany<OutputR*>("*").size() > 1)
        ThrowException("Only one OutputR box is allowed").
                hint("Put all PageR boxes inside the same OutputR box").
                context(this);

    // Find my pages
    _pages = findMany<PageR*>("./*");
    numPages = _pages.size();

    // Open file for R script
    openFile();

    // Get the output path for .txt output
    _filePathTxt = environment().outputFilePath(".txt");
    _filePathTxt.replace("\\", "/");
}

QString OutputR::toScript() {
    QString s;
    for (PageR *page : _pages)
        s += page->toScript();
    s += "plot_all <- function(df) {\n";
    for (PageR *page : _pages) {
        s += "  " + page->functionName() + "(df)\n";
    }
    s += "}\n";

    s += "\nfigures <- function(df) {\n  Pages = list(";
    bool first = true;
    for (PageR *page : _pages) {
        if (!first)
            s += ", ";
        first = false;
        s += toFigureListElement(page);
    }
    s += ")\n}\n";

    return s;
}

QString OutputR::toFigureListElement(PageR *page) {
    QString s = "Page = list(";
    s += "Grob="   + page->functionName() + "(df), ";
    s += "Width="  + page ->port("width" )->value<QString>() + ", ";
    s += "Height=" + page->port("height")->value<QString>();
    s += ")";
    return s;

}

void OutputR::addRCode(QString s) {
    _RCode << s;
}

//
// debrief
//

void OutputR::debrief() {
    writeScript();
    dialog().information("R script written to '" + environment().latestOutputFilePath("R") + "'");
    copyToClipboard();
}

inline QString boolToR(bool x) {
    return x ? "TRUE" : "FALSE";
}

void OutputR::writeScript() {
    QTextStream script(&_file);
    if (!_RCode.isEmpty())
        script << _RCode.join("\n") + "\n";
    script << "show_plots        = " + boolToR(showPlots)     << "\n"
           << "clear_plots       = " + boolToR(clearPlots)    << "\n"
           << "save_data_frame   = " + boolToR(saveDataFrame) << "\n"
           << "skip_formats      = " + boolToR(skipFormats)   << "\n"
           << "box_script_folder = \"" + environment().currentBoxScriptFolder().absolutePath() << "\"\n"
           << "output_file_name  = \"" + _filePathTxt << "\"\n"
           <<
           "sim = read_output(output_file_name)\n"
           "if (save_data_frame) {\n"
           "  file_name_R = paste0(output_file_folder(), \"/\", output_file_base_name(), \".Rdata\")\n"
           "  print(paste(\"Writing sim data frame to\", file_name_R))\n"
           "  save(sim, file=file_name_R)\n"
           "}\n"
           << "\n"
           << toScript()
           << "\n"
           <<
           "if (clear_plots) graphics.off()\n"
           "if (show_plots)  plot_all(sim)\n";
    _file.close();
}

void OutputR::openFile() {
    _filePathR = environment().outputFilePath(".R");
    _filePathR.replace("\\", "/");
    if (_file.isOpen())
        _file.close();
    _file.setFileName(_filePathR);
    if ( !_file.open(QIODevice::WriteOnly | QIODevice::Text) )
        ThrowException("Cannot open file for output").value(_filePathR).context(this);
}

inline QString wrap(QString script) {
    return "source(\"" + environment().inputFileNamePath(script) + "\")\n";
}

void OutputR::copyToClipboard() {
    QString s;

    // Clear memory?
    if (clearMemory)
        s += "rm(list=ls(all=TRUE))\n";

    // Source the default first script
    s += "if (!exists(\"BEGIN\")) " + wrap("scripts/begin.R");

    // Source Sobol scripts?
    bool useSobol = false;
    for (auto plotType : plotTypes) {
        if (plotType.toLower().contains("sobol")) {
            useSobol = true;
            break;
        }
    }
    if (useSobol)
        s += wrap("scripts/begin-sobol.R");

    // Source the generated R script
    s += "source(\"" + _filePathR + "\")\n";

    // Source other scripts
    for (QString script : scripts)
        s += wrap(script);

    // Copy source statements to clipboard
    environment().copyToClipboard(s);
}

}
