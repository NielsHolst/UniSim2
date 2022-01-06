/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_PATH_H
#define BASE_PATH_H

#include <optional>
#include <string>
#include <vector>
#include <QVector>
#include "exception.h"
#include "node.h"

namespace base {


class Path {
public:
    // Types
    using Object  = base::Node;
    using Objects = QList<base::Node*>;

    enum class Directive {
        Any, Children, Descendants, Ancestors, Siblings, Preceding, Following
    };
    // Directive can be Any only for the first Node in an Alternative

    class Alternative;

    class Node {
    public:
        Node(QString directive, QString className, QString objectName);
        Node(std::vector<std::string> names);
        QString toString() const;
    private:
        Directive _directive;
        QString _className, _objectName;
        friend Alternative;
    };

    class Port {
    public:
        enum class Directive {All, Input, Output};
        Port(QString directive, QString name);
        Port(std::vector<std::string> names);
        QString toString() const;
    private:
        Directive _directive;
        QString _name;
        friend Alternative;
    };

    class Alternative {
    public:
        Alternative(Path *parent = nullptr);
        void setParent(Path *parent);
        void setRoot(bool hasRoot);
        void addNode(Node &node);
        void setPort(const Port &port);
        const Objects& matches();
        QString toString() const;

    private:
        // Data
        Path *_parent;
        bool _hasRoot;
        QVector<Node> _nodes;
        std::optional<Port> _port;

        Objects _matches;
        bool _isMatched;

        // Methods
        void initiateMatches(const Node &node);
        void filterMatches  (const Node &node);

        Objects any(Object *p, QString className, QString objectName);

        Objects descendants(Object *p, QString className, QString objectName);
        Objects descendants(Objects candidates, QString className, QString objectName);

        Object* parent (Object *p, QString className, QString objectName);
        Objects parents(Objects candidates, QString className, QString objectName);

        Objects ancestors(Object *p, QString className, QString objectName);
        Objects ancestors(Objects candidates, QString className, QString objectName);

        Objects children(Object *p, QString className, QString objectName);
        Objects children(Objects candidates, QString className, QString objectName);

        Objects siblings(Object *p, QString className, QString objectName);
        Objects siblings(Objects candidates, QString className, QString objectName);

        Object* preceding(Object *p, QString className, QString objectName);
        Objects preceding(Objects candidates, QString className, QString objectName);

        Object* following(Object *p, QString className, QString objectName);
        Objects following(Objects candidates, QString className, QString objectName);
    };

    //
    // Path
    //

    // Methods
    Path(Object *parent=nullptr);
    Path(QString path, Object *parent=nullptr);
    void setParent(Object *parent);
    Object *parent();
    void addAlternative(const Alternative &alternative);
    QString toString() const;
    const Objects& matches();
    template<class T> T findOne();
    template<class T> T findMaybeOne();
    template<class T> QVector<T> findMany();
    static bool isValid(QString path);
private:
    // Data
    Object *_parent;
    QVector<Alternative> _alternatives;

    Objects _matches;
    bool _isMatched;
};

template<class T> inline T Path::findOne() {
    QVector<T> result = findMany<T>();
    if (result.size() != 1)
        ThrowException("Expected exactly one match").value(result.size).context(_parent);
    return result[0];
}

template<class T> inline T Path::findMaybeOne() {
    QVector<T> result = findMany<T>();
    switch (result.size()) {
    case 0: return nullptr;
    case 1: return result[0];
    default: ThrowException("Expected only one match").value(result.size).context(_parent);
    }
}

template<class T> inline QVector<T> Path::findMany() {
    QVector<T> result;
    for (auto candidate : matches()) {
        auto typed = dynamic_cast<T>(candidate);
        if (typed)
            result << typed;
    }
    return result;
}


}

#endif
