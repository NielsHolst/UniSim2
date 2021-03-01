/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_AST_COMMON_H
#define BASE_AST_COMMON_H
#include <string>
#include <boost/optional.hpp>
#include <boost/variant.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <QString>
#include "box_builder.h"

namespace ast {

typedef std::string::const_iterator Iterator;

struct ParameterWithAttribute {
    std::string type, name, attribute;
    QString toString() const;
    void addToBuilder(base::BoxBuilder &builder);
};

struct Parameter {
    ParameterWithAttribute attributedName;
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
