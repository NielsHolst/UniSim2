#ifndef BOXSCRIPT_PARSER_H
#define BOXSCRIPT_PARSER_H
#include <memory>
#include <QString>
#include "boxscript_ast.h"

namespace boxscript{ namespace parser {
    typedef std::shared_ptr<boxscript::ast::boxscript> Result;
    Result parse(std::string source, std::string fileNamePath);
}}

#endif
