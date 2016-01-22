#ifndef BOX_H
#define BOX_H
#include <QMap>
#include <QObject>
#include <QVariant>
#include "box_step.h"
#include "general.h"
#include "port.h"

#define RETURN_PLUGIN_NAME(x) #x
#define Input(X) (*new Port(#X, this)).data(& X).access(Port::ReadWrite)
#define Output(X) (*new Port(#X, this)).data(& X).access(Port::Read).zeroAtReset()

namespace base {

class Port;

class Box : public QObject
{
public:
    Box(QString name, QObject *parent);
    ~Box();
    QString pluginName() const { return RETURN_PLUGIN_NAME(BOXES_PLUGIN_NAME); }
    void addPort(Port *port);
    Port* peakPort(QString name);
    Port* port(QString name);

    static Box* currentRoot();

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
    QString _name;
    QMap<QString,Port*> _ports;
    bool _amended;
    static Box *_currentRoot;
    static bool _currentRootIsDirty;
    // Methods
    void resolvePortImports();
    void allocatePortBuffers();
    void resetPorts();
    void trackPorts(Step step);
    void updateImports();
};

}
#endif
