#include "ast.h"

namespace ast {

//Node _ast;

QString NameValuePair::toString() const {
    return QString::fromStdString(name + "~" + value);
}

QString ParameterWithAttributes::toString() const {
    QString s = QString::fromStdString(name);
    if (attributes.size() > 0) {
        s += " with";
        for (auto attr : attributes)
            s += " " + attr.toString();
    }
    return s;
}

QString Parameter::toString() const {
    QString val = value.is_initialized() ?
                  QString::fromStdString(value.get()) :
                  QString("na");
    return attributedName.toString() + " equals '" + val + "'";
}

QString Node::toString() const {
    QString s;
    addToString(s, 0);
    return s;
}

void Node::clear() {
    className.clear();
    objectName.clear();
    parameters.clear();
    children.clear();
}

void Node::addToString(QString &s, int level) const {
    s += QString().fill(' ', 2*level);
    s += QString::fromStdString(className + " " + objectName);
    s += "\n";
    for (auto parameter : parameters) {
        s += QString().fill(' ', 2*(level+1));
        s += parameter.toString() + "\n";
    }
    for (auto child : children) {
        const Node *node = child.get_pointer();
        node->addToString(s,level+1);
    }
}

bool parse(Iterator begin, Iterator end, Node &result) {
    ast::node_parser<Iterator> parser;
    using boost::spirit::ascii::space;
    bool ok = phrase_parse(begin, end, parser, space, result);
    return (ok && begin == end);
}

} // ast
