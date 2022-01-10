/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_BOX_H
#define BASE_BOX_H
#include <QMap>
#include <QTextStream>
#include <QVector>
#include "caller.h"
#include "node.h"
#include "path.h"
#include "port.h"
#include "port_access.h"

#define Input(X)  (*new base::Port(#X, this)).initialize(& X).access(base::PortAccess::Input)
#define Output(X) (*new base::Port(#X, this)).initialize(& X).access(base::PortAccess::Output).doReset()

#define NamedInput(X,Y)  (*new base::Port((X), this)).initialize(& (Y)).access(base::PortAccess::Input)
#define NamedOutput(X,Y) (*new base::Port((X), this)).initialize(& (Y)).access(PortAccess::Output).doReset()

namespace base {

class Port;
class Timer;

class Box : public Node, public ConstructionStep
{
public:
    Box(QString name, QObject *parent);
    ~Box();
    void addPort(Port *port);

    Port* peakPort(QString name);
    const Port* peakPort(QString name) const;

    Port* port(QString name, Caller caller=Caller());
    const Port* port(QString name, Caller caller=Caller()) const;

    void help(QString s);
    QString help() const;

    void sideEffects(QString s);
    QString sideEffects() const;

    static Box *root();
    Box* boxParent();

    template<class T> T findOne(QString path);
    template<class T> T findMaybeOne(QString path);
    template<class T> QVector<T> findMany(QString path);

    virtual void amend() {}
    virtual void initialize() {}
    virtual void reset() {}
    virtual void update() {}
    virtual void cleanup() {}
    virtual void debrief() {}
    virtual void import() {}
    virtual void run();

    void amendFamily();
    void initializeFamily();
    void resetFamily();
    void updateFamily();
    void cleanupFamily();
    void debriefFamily();

//    virtual void update(Port *) {}
    void enumeratePorts();
    void updatePorts();
    void verifyPorts();

    Box* clone(QString name, QObject *parent);
    Box* cloneFamily(QString name, QObject *parent);
    bool cloned() const;

    static bool debug();
    static void debug(bool on);

    void createTimer(QString name);
    void startTimer(QString name);
    void stopTimer(QString name);

    void toText(QTextStream &text, QString options = "", int indentation = 0) const;
    QString profileReport() const;
private:
    // Data
    QString _help, _sideEffects;
    QMap<QString,Port*> _ports;
//    QVector<Port*> _portsInOrder;
    QVector<Port*> _trackedPorts;
    bool _amended, _cloned, _ignore;
    Timer *_timer;
    static Box *_latest;
    static bool _debugOn, _traceOn;

    // Methods
    Box *findRoot();
    void createTimers();
    void addPort(QMap<QString,Port*> &ports, Port *port);
    void resetPorts();
};

template<class T> inline T Box::findOne(QString path) {
    return Path(path, this).findOne<T>();
}

template<class T> inline T Box::findMaybeOne(QString path) {
    return Path(path, this).findMaybeOne<T>();
}

template<class T> inline  QVector<T> Box::findMany(QString path) {
    return Path(path, this).findMany<T>();
}

}
#endif
