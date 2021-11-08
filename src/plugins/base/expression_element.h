/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef EXPRESSION_ELEMENT_H
#define EXPRESSION_ELEMENT_H

#include <boost/variant/get.hpp>
#include "expression_operand.h"
#include "expression_operator.h"

namespace expression {

enum class Parenthesis{Left, Right};

using ElementVariant = boost::variant<Operand, Operator, Parenthesis>;

class Element : public ElementVariant
{
public:
    // Inherit constructors and operator=
    using ElementVariant::ElementVariant;
    using ElementVariant::operator=;

    enum class Type {Operand, Operator, Parenthesis};
    Type type() const {
        return static_cast<Type>(which());
    }

    QString typeName() const {
        switch(type()) {
        TYPE_NAME(Operand);
        TYPE_NAME(Operator);
        TYPE_NAME(Parenthesis);
        }
    }

    QString toString() const {
        switch(type()) {
        case Type::Operand     : return boost::get<Operand>(*this).toString();
        case Type::Operator    : return expression::toString(boost::get<Operator>(*this));
        case Type::Parenthesis :
            return (boost::get<Parenthesis>(*this) == Parenthesis::Left) ? "(" : ")";
        }
        return QString();
    }
};

}
#endif
