#ifndef GRAPH_H
#define GRAPH_H
#include <base/command.h>

namespace command {

class graph : public base::Command
{
public:
    graph(QString name, QObject *parent);
private:
    void doExecute();
};

}

#endif
