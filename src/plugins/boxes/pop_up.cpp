/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QTextEdit>
#include <base/computation_step.h>
#include <base/dialog.h>
#include <base/exception.h>
#include <base/history.h>
#include <base/publish.h>
#include "pop_up.h"

using namespace base;

namespace boxes {

PUBLISH(PopUp)

PopUp::PopUp(QString name, QObject *parent)
    : Box(name, parent) {
    help("shows message in pop-up window");
    Input(title).help("Title");
    Input(text).help("Short informative text");
    Input(details).help("Further details");
    Input(buttons).equals("OK").help("Vector of buttons to show: (OK Yes No)");
    Input(defaultButton).equals("OK").help("Button assumed when Return is typed");
    Input(escapeButton).equals("OK").help("Button assumed when Esc is typed");
    Input(icon).equals("information").help("Icon to show");
    Input(when).equals("initialize").help("In which step to show message, e.g., \"initialize\" or \"debrief\"");
    Input(show).equals(true).help("Determines whether pop-up will be shown");
    Output(answer).help("Name of button pushed");
    Output(accepted).help("Question accepted?");
    Output(notAccepted).help("Question not accepted?");
}

void PopUp::amend() {
    _error = false;
    try {
        _when = convert<ComputationStep>(when);
    }
    catch (Exception &ex) {
        _error = true;
        _errorMsg = ex.what();
    }
    if (_when == ComputationStep::Amend)
        showPopUp();
}

void PopUp::initialize() {
    // Handle error
    if (_error)
        ThrowException(_errorMsg).context(this);
    // Check buttons (convert and throw away)
    toButtons(buttons);
    toButton(defaultButton);
    toButton(escapeButton);
    // Maybe show
    if (_when==ComputationStep::Initialize && latestCommand()=="run")
        showPopUp();
}

void PopUp::reset() {
    if (_when == ComputationStep::Reset)
        showPopUp();
}

void PopUp::update() {
    if (_when == ComputationStep::Update)
        showPopUp();
}

void PopUp::cleanup() {
    if (_when == ComputationStep::Cleanup)
        showPopUp();
}

void PopUp::debrief() {
    if (_when == ComputationStep::Debrief)
        showPopUp();
}

void PopUp::showPopUp() {
    if (!show)
        return;
    QMessageBox msgBox;
    msgBox.setText(title);
    msgBox.setInformativeText(text);
    msgBox.setDetailedText(details);
    msgBox.setStandardButtons(toButtons(buttons));
    msgBox.setDefaultButton(toButton(defaultButton));
    msgBox.setEscapeButton(toButton(escapeButton));
    msgBox.setIcon(toIcon(icon));
    switch (msgBox.exec()) {
        case QMessageBox::Ok:  answer = "OK"; break;
        case QMessageBox::Yes: answer = "Yes"; break;
        case QMessageBox::No:  answer = "No"; break;
        default: ;// Never reached
    }
    accepted = (answer != "No");
    notAccepted = !accepted;
}

QMessageBox::StandardButton PopUp::toButton(QString source) {
    if (source == "OK")  return  QMessageBox::Ok;
    if (source == "Yes") return QMessageBox::Yes;
    if (source == "No")  return  QMessageBox::No;
    ThrowException("Unknown button").value(source).context(this);
}

QMessageBox::StandardButtons PopUp::toButtons(QVector<QString> source) {
    QMessageBox::StandardButtons buttons = QMessageBox::StandardButtons();
    for (QString s : source)
        buttons = buttons | toButton(s);
    return buttons;
}

QMessageBox::Icon PopUp::toIcon(QString source) {
    QString s = source.toLower();
    if (s == "none") return QMessageBox::NoIcon;
    if (s == "question") return QMessageBox::Question;
    if (s == "information") return QMessageBox::Information;
    if (s == "warning") return QMessageBox::Warning;
    if (s == "critical") return QMessageBox::Critical;
    ThrowException("Unknown icon").value(source).context(this);
}

QString PopUp::latestCommand() const {
    QString s = dialog().history()->previous();
    return s.trimmed().split(" ").at(0).toLower();
}

} //namespace

