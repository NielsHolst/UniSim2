#ifndef AST_COMMON_H
#define AST_COMMON_H
#include <string>
#include <boost/optional.hpp>
#include <boost/variant.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <QString>
#include "box_builder.h"

namespace ast {

typedef std::string::const_iterator Iterator;
//typedef boost::optional<std::string> OptionalValue;

struct NameValuePair {
    std::string name, value;
    QString toString() const;
    void addToBuilder(base::BoxBuilder &builder);
};

struct ParameterWithAttributes {
    std::string type, name;
    std::vector<NameValuePair> attributes;
    QString toString() const;
    void addToBuilder(base::BoxBuilder &builder);
};

struct Parameter {
    ParameterWithAttributes attributedName;
    std::string value;
    QString toString() const;
    void addToBuilder(base::BoxBuilder &builder);
};

struct Node;
typedef boost::recursive_wrapper<Node> CompositeNode;

struct Node {
    std::string className, objectName;
    std::vector<Parameter> parameters;
    std::vector<CompositeNode> children;
    void clear();
    QString toString() const;
    void addToBuilder(base::BoxBuilder &builder);
private:
    void addToString(QString &s, int level) const;
};

}

#endif
