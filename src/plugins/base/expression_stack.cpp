/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stack>
#include <boost/variant/get.hpp>
#include "bare_date.h"
#include "bare_date_time.h"
#include "expression_operation.h"
#include "expression_operator.h"
#include "expression_stack.h"
#include "port.h"

using namespace base;
using boost::get;

namespace expression {

Stack::Stack() {

}

void Stack::clear() {
    _elements.clear();
}

void Stack::push(Element element) {
    _elements.push_back(element);
}

void Stack::push(Operator operator_) {
    _elements.push_back(operator_);
}

void Stack::push(Operand operand) {
    switch(operand.type()) {
    case Operand::Type::Bool:            _elements.push_back(get<bool>(operand)); break;
    case Operand::Type::Char:            _elements.push_back(get<char>(operand)); break;
    case Operand::Type::Int:             _elements.push_back(get<int>(operand)); break;
    case Operand::Type::Double:          _elements.push_back(get<double>(operand)); break;
    case Operand::Type::Date:            _elements.push_back(get<QDate>(operand)); break;
    case Operand::Type::DateTime:        _elements.push_back(get<QDateTime>(operand)); break;
    case Operand::Type::Time:            _elements.push_back(get<QTime>(operand)); break;
    case Operand::Type::BareDate:        _elements.push_back(get<base::BareDate>(operand)); break;
    case Operand::Type::BareDateTime:    _elements.push_back(get<base::BareDateTime>(operand)); break;
    case Operand::Type::String:          _elements.push_back(get<QString>(operand)); break;
    case Operand::Type::PortPath:        _elements.push_back(get<QString>(operand)); break;
    case Operand::Type::BoolPtr:         _elements.push_back(get<bool*>(operand)); break;
    case Operand::Type::CharPtr:         _elements.push_back(get<char*>(operand)); break;
    case Operand::Type::IntPtr:          _elements.push_back(get<int*>(operand)); break;
    case Operand::Type::DoublePtr:       _elements.push_back(get<double*>(operand)); break;
    case Operand::Type::DatePtr:         _elements.push_back(get<QDate*>(operand)); break;
    case Operand::Type::DateTimePtr:     _elements.push_back(get<QDateTime*>(operand)); break;
    case Operand::Type::TimePtr:         _elements.push_back(get<QTime*>(operand)); break;
    case Operand::Type::BareDatePtr:     _elements.push_back(get<base::BareDate*>(operand)); break;
    case Operand::Type::BareDateTimePtr: _elements.push_back(get<base::BareDateTime*>(operand)); break;
    case Operand::Type::StringPtr:       _elements.push_back(get<QString*>(operand)); break;
    case Operand::Type::PortPtr:         _elements.push_back(get<Port*>(operand)); break;
    case Operand::Type::PortPtrVector:   _elements.push_back(get<QVector<Port*>>(operand)); break;
    }
}

void Stack::push(Parenthesis parenthesis) {
    _elements.push_back(parenthesis);
}

const Element & Stack::at(int i) const {
    return _elements.at(i);
}

bool Stack::isEmpty() const {
    return _elements.empty();
}

bool Stack::containsPortPaths() const {
    for (Element el : _elements) {
        if (el.type() == Element::Type::Operand) {
           Operand op = get<Operand>(el);
            if (op.type() == Operand::Type::PortPath)
                return true;
        }
    }
    return false;
}

void Stack::evaluate() {
    _stack.clear();
    for (ElementsIt it=_elements.begin(); it!=_elements.end(); ++it) {
        if (it->type()==Element::Type::Operator)
            reduce(get<Operator>(*it));
        else
            _stack.push_back(get<Operand>(*it));
    }
    if (_stack.size() == 0)
        ThrowException("Too many operators for stack");
    if (_stack.size() > 1)
        ThrowException("Too few operators for stack");
    _value = _stack.front();
}

void Stack::reduce(Operator op) {
    // Take top operand
    Operand prevTop = _stack.back(),
            result;
    _stack.pop_back();

    // Apply operator on prevTop and stack
    switch(op) {
    case Operator::Addition:
        result = add(prevTop, _stack.back());
        break;
    case Operator::Subtraction:
        result = subtract(prevTop, _stack.back());
        break;
    case Operator::Multiplication:
        result = multiply(prevTop, _stack.back());
        break;
    case Operator::Division:
        result = divide(prevTop, _stack.back());
        break;
    case Operator::Exponentiation:
    case Operator::And:
    case Operator::Or:
    case Operator::Union:
    case Operator::If:
    case Operator::Else: ;
    }
    _stack.back() = result;
}

inline bool isLeftPar(const Element &element) {
    return
      (element.type() == Element::Type::Parenthesis) &&
      (get<Parenthesis>(element) == Parenthesis::Left);
}

inline bool precedence(const Element &element) {
    if (element.type() == Element::Type::Operator)
        return precedence(get<Operator>(element));
    return 0;
}

void Stack::toPostfix() {
    // From: https://www.tutorialspoint.com/Convert-Infix-to-Postfix-Expression
    Elements postfix, stk;
    Operator operator_;
    Parenthesis parenthesis;
    for (auto element : _elements) {
        switch (element.type()) {
        case Element::Type::Operand:
            postfix.push_back(element);
            break;
        case Element::Type::Parenthesis:
            parenthesis = get<Parenthesis>(element);
            if (parenthesis == Parenthesis::Left)
                stk.push_back(element);
            else { // Parenthesis::Right
                // Pop until matching left parenthesis
                while (!(isLeftPar(stk.front()))) {
                    postfix.push_back( stk.back() );
                    stk.pop_back();
                }
                stk.pop_back(); // pop left parenthesis
                if (stk.empty())
                    ThrowException("No matching left parenthesis");
            }
            break;
        case Element::Type::Operator:
            operator_ = get<Operator>(element);
            if (stk.empty() || precedence(operator_) > precedence(stk.back()))
                stk.push_back(element);
            else {
                // Pop until a higher precedence is found
                while(!stk.empty() && precedence(operator_) <= precedence(stk.back())) {
                    postfix.push_back( stk.back() );
                    stk.pop_back();
                }
                stk.push_back(element);
            }
            break;
        }
    } // for
    // Pop until stack is empty
    while (!stk.empty()) {
        postfix.push_back( stk.back() );
        stk.pop_back();
    }
    // Cope result
    _elements = postfix;
}

QString Stack::toString() const {
    QStringList str;
    for (auto &element : _elements)
        str << element.toString();
    return str.join(" ");
}

}
