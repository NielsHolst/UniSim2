/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "composite_path.h"

using namespace std;

namespace base {


CompositePath::CompositePath(const QObject *parent)
    : _parent(parent)
{
}

void CompositePath::addPath(const Path &path) {
    _paths << path;
}

void CompositePath::resolve() {
    Path::Objects pool;
    for (auto path : _paths) {
//        path.include(pool, )
    }
}

//Path::Path(boxscript::ast::ReferenceUnion ru, const QObject *context)
//    : Path(context)
//{
//    for (auto &ref : ru.references) {
//        Trail trail;
//        for (auto &e : ref.path.elements) {
//            Trail::Leg leg;
//            bool hasDirective = (directives.contains(qstr(e.at(0))));
//            Directive dir = hasDirective ? directives.value(qstr(e.at(0))) : Directive::None;
//            switch (e.size()) {
//            case 1:
//                leg = Trail::Leg{Directive::None, "Box", qstr(e.at(0))};
//                break;
//            case 2:
//                leg = hasDirective ? Trail::Leg{dir,             "Box",         qstr(e.at(1))} :
//                                     Trail::Leg{Directive::None, qstr(e.at(0)), qstr(e.at(1))};
//                break;
//            case 3:
//                if (!hasDirective)
//                    ThrowException("Unknown path directive").value(qstr(e.at(0)));
//                leg = Trail::Leg{dir, qstr(e.at(1)), qstr(e.at(2))};
//                break;

//            }
//            trail.hasRoot = ref.path.root.has_value();
//            trail.legs << leg;
//        }
//        if (!ref.port.empty())
//            trail.legs << Trail::Leg{Directive::None, "Port", qstr(ref.port)};
//        _trails << trail;
//    }
//}

} //namespace
