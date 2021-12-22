#ifndef BOXSCRIPT_PARSER_H
#define BOXSCRIPT_PARSER_H
#include <memory>
#include <QString>
#include "boxscript_ast.h"
#include "expression.h"

namespace boxscript{ namespace parser {
    typedef std::shared_ptr<boxscript::ast::boxscript> Result;
    Result parse(std::string source, std::string fileNamePath = "");
    base::Expression parseExpression(QString s);
}}

#endif
