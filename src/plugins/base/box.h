#ifndef BOX_H
#define BOX_H
#include <QMap>
#include <QObject>
#include <QVariant>
#include <QVector>
#include "box_step.h"
#include "general.h"
#include "path.h"
#include "port.h"

#define RETURN_PLUGIN_NAME(x) #x

#define Input(X) (*new Port(#X, this)).data(& X).access(Port::Input)
#define Output(X) (*new Port(#X, this)).data(& X).access(Port::Output).zeroAtReset()

namespace base {

class Port;
class Timer;

class Box : public QObject
{
public:
    Box(QString name, QObject *parent);
    ~Box();
    QString pluginName() const { return RETURN_PLUGIN_NAME(BOXES_PLUGIN_NAME); }
    void addPort(Port *port);
    void addOrphanPort(Port *port);
    Port* peakPort(QString name);
    const Port* peakPort(QString name) const;
    Port* port(QString name);
    const Port* port(QString name) const;

    void help(QString s);
    QString help() const;
    void sideEffects(QString s);
    QString sideEffects() const;

    static Box* currentRoot();
    static void saveCurrentRoot();
    static void restoreCurrentRoot();

    QString className() const;
    QString fullName() const;
    int order() const;
    static int count();
    QString profileReport() const;

    template<class T=QObject> T* resolveOne(QString path);
    template<class T=QObject> T* resolveMaybeOne(QString path);
    template<class T=QObject> QVector<T*> resolveMany(QString path);

    virtual void amend() {}
    virtual void initialize() {}
    virtual void reset() {}
    virtual void update() {}
    virtual void cleanup() {}
    virtual void debrief() {}
    virtual void run();

    void amendFamily();
    void initializeFamily();
    void resetFamily();
    void updateFamily();
    void cleanupFamily();
    void debriefFamily();

private:
    // Data
    QString _name, _help, _sideEffects;
    QMap<QString,Port*> _ports, _orphanPorts;
    QVector<Port*> _trackedPorts;
    int _order;
    bool _amended;
    Timer *_timer;
    static Box *_currentRoot, *_savedCurrentRoot;
    static int _count;
    // Methods
    void createTimers();
    void addPort(QMap<QString,Port*> &ports, Port *port);
    void postAmend();
    void enumerateBoxes(int &i);
    void resolvePortImports();
    void allocatePortBuffers();
    void collectTrackedPorts();
    void resetPorts();
    void trackPorts(Step step);
    void updateImports();
};

template<class T> T* Box::resolveOne(QString path) {
    return Path(path, this).resolveOne<T>(this);
}

template<class T> T* Box::resolveMaybeOne(QString path) {
    return Path(path, this).resolveMaybeOne<T>(this);
}

template<class T> QVector<T*> Box::resolveMany(QString path) {
    return Path(path, this).resolveMany<T>(this);
}

}
#endif
