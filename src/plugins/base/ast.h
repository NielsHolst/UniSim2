#ifndef AST_H
#define AST_H
#include <boost/optional.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <QString>

namespace ast {

    struct NameValuePair {
        std::string name, value;
        QString toString() const;
    };

    struct ParameterWithAttributes {
        std::string name;
        std::vector<NameValuePair> attributes;
        QString toString() const;
    };

    struct Parameter {
        ParameterWithAttributes attributedName;
//        boost::optional<std::string> value;
        std::string value;
        QString toString() const;
    };

    struct Node;
    typedef boost::recursive_wrapper<Node> CompositeNode;

    struct Node {
        std::string className, objectName;
        std::vector<Parameter> parameters;
        std::vector<CompositeNode> children;
        QString toString() const;
        void clear();
    private:
        void addToString(QString &s, int level) const;
    };

    extern Node _ast;

} //ast


#endif
