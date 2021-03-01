/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_BOX_H
#define BASE_BOX_H
#include <QMap>
#include <QObject>
#include <QTextStream>
#include <QVector>
#include "caller.h"
#include "path.h"
#include "port.h"
#include "port_access.h"

#define RETURN_PLUGIN_NAME(x) #x

#define Input(X)  (*new base::Port(#X, this)).data(& X).access(base::PortAccess::Input)
#define Output(X) (*new base::Port(#X, this)).data(& X).access(base::PortAccess::Output).zeroAtReset()

#define NamedInput(X,Y)  (*new base::Port((X), this)).data(& (Y)).access(base::PortAccess::Input)
#define NamedOutput(X,Y) (*new base::Port((X), this)).data(& (Y)).access(PortAccess::Output).zeroAtReset()

namespace base {

class Port;
class Timer;

class Box : public QObject, public ConstructionStep
{
public:
    enum Persistence {DontAllowRemoval, AllowRemoval};
    Box(QString name, QObject *parent, Persistence=DontAllowRemoval);
    ~Box();
    QString pluginName() const { return RETURN_PLUGIN_NAME(BOXES_PLUGIN_NAME); }
    void addPort(Port *port);

    Port* peakPort(QString name);
    const Port* peakPort(QString name) const;

    Port* port(QString name, Caller caller=Caller());
    const Port* port(QString name, Caller caller=Caller()) const;

    void help(QString s);
    QString help() const;

    void sideEffects(QString s);
    QString sideEffects() const;

    void removeChild(QString name);

    static Box* currentRoot();
    static void saveCurrentRoot();
    static void restoreCurrentRoot();

    Box* boxParent();

    QString className() const;
    QString name() const;
    QString fullName() const;
    int order() const;
    static int count();
    QString profileReport() const;

    template<class T=QObject> T* findOne(QString path);
    template<class T=QObject> T* findMaybeOne(QString path);
    template<class T=QObject> QVector<T*> findMany(QString path);

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

    virtual void update(Port *) {}
    void resolvePortImports();
    void updateImports();
    void checkSelfImports();
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
private:
    // Inputs
    bool remove;
    // Data
    QString _help, _sideEffects;
    QMap<QString,Port*> _ports, _orphanPorts;
    QVector<Port*> _trackedPorts;
    int _order;
    bool _amended, _cloned, _ignore;
    Timer *_timer;
    static Box *_currentRoot, *_savedCurrentRoot;
    static int _count;
    static bool _debugOn, _traceOn;
    // Methods
    void createTimers();
    void addPort(QMap<QString,Port*> &ports, Port *port);
    void enumerateBoxes(int &i);
    void resetPorts();
};

template<class T> T* Box::findOne(QString path) {
    return Path(path, this).resolveOne<T>(this);
}

template<class T> T* Box::findMaybeOne(QString path) {
    return Path(path, this).resolveMaybeOne<T>(this);
}

template<class T> QVector<T*> Box::findMany(QString path) {
    return Path(path, this).resolveMany<T>(this);
}

}
#endif
