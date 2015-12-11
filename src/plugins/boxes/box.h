#ifndef BOX_H
#define BOX_H
#include <QMap>
#include <QObject>
#include <QVariant>
#include "general.h"

#define RETURN_PLUGIN_NAME(x) #x
#define Input(X) (*new Port(#X, this)).data(& X).access(Port::ReadWrite)
#define Output(X) (*new Port(#X, this)).data(& X).access(Port::Read)

namespace boxes {

class Port;

class Box : public QObject
{
public:
    Box(QString name, QObject *parent);
    ~Box();
    QString pluginName() const { return RETURN_PLUGIN_NAME(BOXES_PLUGIN_NAME); }
    void addPort(Port *port);
    void run();
    static Box* currentRoot();
    Port* peekPort(QString name);
    Port* seekPort(QString name);

private:
    // Inputs
    int iterations, steps;
    // Outputs
    int iteration, step;
    // Data
    QString _name;
    bool amended;
    QMap<QString,Port*> ports;

    static Box *_currentRoot;
    static bool _currentRootIsDirty;

    // Methods
    virtual void amend() {}
    virtual void initialize();
    virtual void reset();
    virtual void update() {}
    virtual void cleanup() {}
    virtual void debrief() {}
    void updateImports();
    void doAmend();
    void doInitialize();
    void doReset();
    void doUpdate();
    void doCleanup();
    void doDebrief();

    template <class T> static QList<T> filterSearch(QString name, const QList<QObject*> &candidates);
};

}
#endif
