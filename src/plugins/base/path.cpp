/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <algorithm>
#include <QMap>
#include "box.h"
#include "boxscript_parser.h"
#include "expression.h"
#include "path.h"

using namespace std;

namespace base {

// Lookup tables

static QMap<QString, Path::Directive> nodeDirFromString =
{
    {"any",         Path::Directive::Any},
    {"children",    Path::Directive::Children},
    {"descendants", Path::Directive::Descendants},
    {"ancestors",   Path::Directive::Ancestors},
    {"siblings",    Path::Directive::Siblings},
    {"preceding",   Path::Directive::Preceding},
    {"following",   Path::Directive::Following}
};

static QMap<Path::Directive, QString> nodeDirToString =
{
    {Path::Directive::Any        , "any"        },
    {Path::Directive::Children   , "children"   },
    {Path::Directive::Descendants, "descendants"},
    {Path::Directive::Ancestors  , "ancestors"  },
    {Path::Directive::Siblings   , "siblings"   },
    {Path::Directive::Preceding  , "preceding"  },
    {Path::Directive::Following  , "following"  }
};

static QMap<QString, Path::Port::Directive> portDirFromString =
{
    {"all",    Path::Port::Directive::All},
    {"input",  Path::Port::Directive::Input},
    {"output", Path::Port::Directive::Output}
};

static QMap<Path::Port::Directive, QString> portDirToString =
{
    {Path::Port::Directive::All   , "all"   },
    {Path::Port::Directive::Input , "input" },
    {Path::Port::Directive::Output, "output"}
};

inline bool isNodeDirective(std::string s) {
    return nodeDirFromString.contains(QString::fromStdString(s));
}

inline bool isNodeDirective(QString s) {
    return nodeDirFromString.contains(s);
}

inline bool isPortDirective(std::string s) {
    return portDirFromString.contains(QString::fromStdString(s));
}

inline bool isPortDirective(QString s) {
    return portDirFromString.contains(s);
}

inline QString qstr(std::string s) {
    return QString::fromStdString(s);
}

// Node

Path::Node::Node(QString directive, QString className, QString objectName)
    : _className(className), _objectName(objectName)
{
    if (directive.isEmpty())
        _directive =  Directive::Any;
    else if (isNodeDirective(directive))
        _directive = nodeDirFromString.value(directive);
    else
        ThrowException("Unknown node directive").value(directive);
}

Path::Node::Node(std::vector<std::string> names) {
    // Must have at least one name
    if (names.empty())
        ThrowException("Empty name in path");

    // Directive, if present, must be first name
    bool hasDirective = isNodeDirective(names.at(0));

    // If no directive, the node refers to Any by default
    _directive = hasDirective ?
         nodeDirFromString.value(qstr(names.at(0))) :
         base::Path::Directive::Any;

    // Set class and object name
    bool tooLong = false;
    switch (names.size()) {
    case 1:
        // Object name only
        _objectName = qstr(names.at(0));
        break;
    case 2:
        if (hasDirective)
            // Directive + object name
            _objectName = qstr(names.at(1));
        else {
            // Class name, + object name
            _className  = qstr(names.at(0));
            _objectName = qstr(names.at(1));
        }
        break;
    case 3:
        if (hasDirective) {
            // Directive + class name + object name
            _className  = qstr(names.at(1));
            _objectName = qstr(names.at(2));
        }
        else {
            // (Namespace + class name) + object name
            _className  = qstr(names.at(0)) + "::" + qstr(names.at(1));
            _objectName = qstr(names.at(2));
        }
        break;
    case 4:
        if (hasDirective) {
            // Directive + (namespace + class name) + object name
            _className  = qstr(names.at(1)) + "::" + qstr(names.at(2));
            _objectName = qstr(names.at(3));
        }
        else
            ThrowException("Unknown path directive").value(qstr(names.at(0)));
        break;
    default:
        tooLong = true;
    }
    if (tooLong) {
        QStringList list;
        for (auto s : names)
            list << qstr(s);
        ThrowException("Too many tokens (>4) in path").value(list.join("::")).value2(names.size());
    }
}

QString Path::Node::toString() const {
    QString result = (_directive == Directive::Any) ? "" : (nodeDirToString.value(_directive) + "::");
    if (!_className.isEmpty())
        result += _className + "::";
    return result + _objectName;
}

// Port

Path::Port::Port(QString directive, QString name)
    : _name(name)
{
    if (directive.isEmpty())
        _directive = Directive::All;
    else if (portDirFromString.contains(directive))
        _directive = portDirFromString.value(directive);
    else
        ThrowException("Unknown port directive").value(directive);
}

Path::Port::Port(std::vector<std::string> names) {
    // Must have at least one name
    if (names.empty())
        ThrowException("Empty port name");

    // Directive, if present, must be first name
    bool hasDirective = isPortDirective(names.at(0));

    // If no directive, the port refers to any kind of port
    _directive = hasDirective ?
         portDirFromString.value(qstr(names.at(0))) : base::Path::Port::Directive::All;

    // Set name
    bool tooLong = false;
    switch (names.size()) {
    case 1:
        // Only name
        if (hasDirective)
            ThrowException("Directive cannot be used alone").value(qstr(names.at(0)));
        _name = qstr(names.at(0));
        break;
    case 2:
        if (hasDirective)
            // Directive + name
            _name = qstr(names.at(1));
        else {
            ThrowException("Unknown path directive").value(qstr(names.at(0)));
        }
        break;
    default:
        tooLong = true;
    }
    if (tooLong) {
        QStringList list;
        for (auto s : names)
            list << qstr(s);
        ThrowException("Too many tokens (>2) in port").value(list.join("::")).value2(names.size());
    }

}

QString Path::Port::toString() const {
    QString result = "[";
    if (_directive != Directive::All)
        result += portDirToString.value(_directive) + "::";
    return result + _name + "]";
}

// Alternative

Path::Alternative::Alternative() {
}

void Path::Alternative::setRoot(bool hasRoot) {
    _hasRoot = hasRoot;
}

void Path::Alternative::addNode(Node &node) {
    // An empty directive means Any for the first node and, Children for the following nodes
    if (!_nodes.isEmpty() && node._directive==Directive::Any)
        node._directive = Directive::Children;
    _nodes << node;
}

void Path::Alternative::setPort(const Port &port) {
    // Dummy port is ignored
    if (_port->_name != "DUMMY")
        _port = port;
}

bool Path::Alternative::hasRoot() const {
    return _hasRoot;
}

const QVector<Path::Node> &Path::Alternative::nodes() const {
    return _nodes;
}

const std::optional<Path::Port> &Path::Alternative::port() const {
    return _port;
}

QString Path::Alternative::toString() const {
    QStringList list;
    for (auto node : _nodes)
        list <<node.toString();

    QString result = _hasRoot ? "/" : "";
    result += list.join("/");
    if (_port.has_value())
        result += _port->toString();
    return result;
}

const Path::Objects& Path::Alternative::matches(base::Node *parent) {
    auto next = _nodes.begin();
    if (next) {
        initiateMatches(*next, parent);
        ++next;
    }
    for(;next != _nodes.end(); ++next)
        filterMatches(*next);

    Objects ports;
    if (_port.has_value()) {
        for (auto match : _matches) {
            for (auto child : match->children<base::Port*>()) {
                if (child->name() == (*_port).name())
                    ports << dynamic_cast<base::Node*>(child);
            }
        }
        _matches = ports;
    }
    return _matches;
}

void Path::Alternative::initiateMatches(const Node &node, base::Node *parentNode) {
    // Find root and parent
    Object *root = Box::root();

    // Check that root or parent exists, if need
    enum {NeedsRoot, NeedsParent} need;
//    need = (_hasRoot || node._directive==Directive::Any) ? NeedsRoot : NeedsParent;
    need = (node._objectName.contains(".") || node._directive!=Directive::Any) ? NeedsParent : NeedsRoot;

    // Check root
    if (need==NeedsRoot && !root)
        ThrowException("No root found for path").value(toString()).context(parentNode);

    // Check parent
    if (need==NeedsParent && !parentNode)
        ThrowException("No parent found for path").value(toString()).context(parentNode);

    // Set up initial nodes for seach
    if (_hasRoot)
        // Search from the root's children
        _matches << children(root, node._className, node._objectName);
    else if (node._objectName == ".")
        // Search from parent node
        _matches << parentNode;
    else if (node._objectName == "..")
        // Search from grandparent node
        _matches << parent(parentNode, "", "*");
    else if (node._objectName == "...")
        // Search from nearest nodes
        _matches << parentNode << ancestors(parentNode, "", "*");
    else {
        switch(node._directive) {
        case Directive::Any:
            // Search from any node matching class and object name
            _matches << any(root, node._className, node._objectName);
            break;
        case Directive::Children:
            // Search from all parent's children
            _matches << children(parentNode, node._className, node._objectName);
            break;
        case Directive::Descendants:
            // Search from parent's descendants
            _matches << descendants(parentNode, node._className, node._objectName);
            break;
        case Directive::Ancestors:
            // Search from parent's ancestors
            _matches << ancestors(parentNode, node._className, node._objectName);
            break;
        case Directive::Siblings:
            // Search from parent's siblings
            _matches << siblings( parentNode, node._className, node._objectName);
            break;
        case Directive::Preceding:
            // Search from parent's preceding sibling
            _matches << preceding(parentNode, node._className, node._objectName);
            break;
        case Directive::Following:
            // Replace each match with its following sibling
            _matches << following(parentNode, node._className, node._objectName);
            break;
        }
    }
}

void Path::Alternative::filterMatches(const Node &node) {
    if (node._objectName == ".") {
        // Nothing to do
        ;
    }
    else if (node._objectName == "..") {
        // Replace matches with their parents
        _matches = parents(_matches, "", "*");
    }
    else if (node._objectName == "...") {
        // Add ancestors to matches
        _matches << ancestors(_matches, "", "*");
    }
    else {
        switch(node._directive) {
        case Directive::Any:
            ThrowException("Any direction is invalid here").value(toString());
        case Directive::Children:
            // Replace matches with their children
            _matches = children(_matches, node._className, node._objectName);
            break;
        case Directive::Descendants:
            // Replace matches with their descendants
            _matches = descendants(_matches, node._className, node._objectName);
            break;
        case Directive::Ancestors:
            // Replace matches with their ancestors
            _matches = ancestors(  _matches, node._className, node._objectName);
            break;
        case Directive::Siblings:
            // Replace matches with their siblings
            _matches = siblings(   _matches, node._className, node._objectName);
            break;
        case Directive::Preceding:
            // Replace each match with its preceding sibling
            _matches = preceding(  _matches, node._className, node._objectName);
            break;
        case Directive::Following:
            // Replace each match with its following sibling
            _matches = following(  _matches, node._className, node._objectName);
            break;
        }
    }
}

bool match(Path::Object *p, QString className, QString objectName) {
    return (className.isEmpty() || p->isType(className)) &&
           (objectName == "*"   || p->name() == objectName);
}

Path::Objects Path::Alternative::any(Object *p, QString className, QString objectName) {
    auto desc = p->descendants<Object*>();
    Objects all, result;
    all << p << Objects(desc.begin(), desc.end());
    for (auto candidate : all) {
        if (match(candidate, className, objectName))
            result << candidate;
    }
    return result;
}

Path::Objects Path::Alternative::descendants(Object *p, QString className, QString objectName) {
    Objects result;
    auto descendants = p->descendants<Object*>();
    for (auto descendant : descendants) {
        if (match(descendant, className, objectName))
            result << descendant;
    }
    return result;
}

Path::Objects Path::Alternative::descendants(Objects candidates, QString className, QString objectName) {
    Objects result;
    for (auto candidate : candidates)
        result << descendants(candidate, className, objectName);
    return result;
}

Path::Object* Path::Alternative::parent(Object *p, QString className, QString objectName) {
    auto parent = p->parent<Object*>();
    return (parent && match(parent, className, objectName)) ? parent : nullptr;
}

Path::Objects Path::Alternative::parents(Objects candidates, QString className, QString objectName) {
    Objects result;
    for (auto candidate : candidates) {
        auto p = parent(candidate, className, objectName);
        if (p)
            result << p;
    }
    return result;
}

Path::Objects Path::Alternative::ancestors(Object *p, QString className, QString objectName) {
    Objects result;
    auto q = dynamic_cast<Object*>(p->parent());
    if (q) {
        if (match(q, className, objectName))
            result << q;
        result << ancestors(q, className, objectName);
    }
    return result;
}

Path::Objects Path::Alternative::ancestors(Objects candidates, QString className, QString objectName) {
    Objects result;
    for (auto candidate : candidates)
        result << ancestors(candidate, className, objectName);
    return result;
}

Path::Objects Path::Alternative::children(Object *p, QString className, QString objectName) {
    Objects result;
    auto children = p->children<Object*>();
    for (auto child : children) {
        if (match(child, className, objectName))
            result << child;
    }
    return result;
}

Path::Objects Path::Alternative::children(Objects candidates, QString className, QString objectName) {
    Objects result;
    for (auto candidate : candidates)
        result << children(candidate, className, objectName);
    return result;
}

Path::Objects Path::Alternative::siblings(Object *p, QString className, QString objectName) {
    Objects result;
    auto children = p->children<Object*>();
    for (auto child : children) {
        if (child != p && match(child, className, objectName))
            result << child;
    }
    return result;
}

Path::Objects Path::Alternative::siblings(Objects candidates, QString className, QString objectName) {
    Objects result;
    for (auto candidate : candidates)
        result << siblings(candidate, className, objectName);
    return result;
}

Path::Object* Path::Alternative::preceding(Object *p, QString className, QString objectName) {
    auto children = p->children<Object*>();
    int i=0;
    while (children.at(i) != p) ++i;
    return (i>0 && match(children.at(i), className, objectName)) ? children[i] : nullptr;
}

Path::Objects Path::Alternative::preceding(Objects candidates, QString className, QString objectName) {
    Objects result;
    for (auto candidate : candidates) {
        auto p = preceding(candidate, className, objectName);
        if (p)
            result << p;
    }
    return result;
}

Path::Object* Path::Alternative::following(Object *p, QString className, QString objectName) {
    auto children = p->children<Object*>();
    int i=0,
        n=children.size();
    while (children.at(i) != p) ++i;
    return (i<n-1 && match(children.at(i+1), className, objectName)) ? children[i+1] : nullptr;
}

Path::Objects Path::Alternative::following(Objects candidates, QString className, QString objectName) {
    Objects result;
    for (auto candidate : candidates) {
        auto p = following(candidate, className, objectName);
        if (p)
            result << p;
    }
    return result;
}

// Path

Path::Path(Object *parent)
    : _parent(parent)
{
}

Path::Path(QString path, Object *parent)
    : Path(parent)
{
    // The parser needs a port on every alternative: Add missing ports as dummies
    QStringList alternatives = path.split("|"), corrected;
    for (auto alternative : alternatives)
        corrected << (path.contains("[") ? alternative : alternative.trimmed() + "[DUMMY]");

    // Parse path as an expression
    auto expression = boxscript::parser::parseExpression(corrected.join("|"));
    if (expression.size() != 1 || Expression::type(expression.at(0)) != Expression::Type::Path)
        ThrowException("Invalid path").value(path).context(parent);

    Path parsedPath = std::get<Path>(expression.at(0));
    _parent = parent ? parent : parsedPath._parent;
    _alternatives = parsedPath._alternatives;
    _matches = parsedPath._matches;
}

void Path::setParent(Object *parent)
{
    _parent = parent;
}

Path::Object *Path::parent() {
    return _parent;
}

void Path::addAlternative(Alternative alternative) {
    _alternatives << alternative;
}

const Path::Objects &Path::matches() const {
    // Path must have a parent
    if (!_parent)
        ThrowException("Path must have a parent").value(toString());

    // Empty path is illegal
    if (_alternatives.isEmpty())
        ThrowException("Path is empty").context(_parent);

    // Collect matches
    for (auto &alternative : _alternatives) {
        for (auto match : alternative.matches(_parent)) {
            // Avoid duplicates
            if (!_matches.contains(match))
                _matches << match;
        }
    }

    // Sort matches
    std::sort(_matches.begin(), _matches.end());
    return _matches;
}

QString Path::toString() const {
    QStringList list;
    for (auto alt : _alternatives)
        list <<alt.toString();
    return list.join("|");
}

const QVector<Path::Alternative> &Path::alternatives() const {
    return _alternatives;
}

bool Path::isValid(QString path) {
    auto expression = boxscript::parser::parseExpression(path);
    return (expression.size() == 1 &&
            Expression::type(expression.at(0)) == Expression::Type::Path);

}


} //namespace
