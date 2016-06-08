#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/publish.h>
#include <base/save_graph_graphviz.h>
#include "graph.h"

using namespace base;

namespace command {

PUBLISH(graph)
HELP(graph, "graph", "generate and show graph")

graph::graph(QString name, QObject *parent)
    : Command(name, parent)
{
}

void graph::doExecute() {
    if (_args.size() == 1) {
        SaveGraphGraphviz().writeGraphFile();
        SaveGraphGraphviz().showGraphFile();
        environment().incrementFileCounter();
    }
    else
        dialog().error("Just write: 'graph'; it takes no arguments");
}



}
