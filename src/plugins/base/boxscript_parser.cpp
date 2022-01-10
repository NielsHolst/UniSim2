#include <QFile>
#include "box.h"
#include "boxscript_config.h"
#include "boxscript_parser.h"
#include "exception.h"

using namespace base;

namespace boxscript { namespace parser {

Result parse(std::string source, std::string fileNamePath) {
    // Set up iterators
    using boxscript::parser::iterator_type;
    iterator_type iter(source.begin());
    iterator_type const end(source.end());

    // Our AST
    auto ast = std::make_shared<ast::boxscript>();

    // Our error handler
    using boxscript::parser::error_handler_type;
    using boxscript::parser::error_handler_tag;
    std::stringstream errorMsg;
    error_handler_type error_handler(iter, end, errorMsg, fileNamePath);

    // Our parser
    auto const parser =
        x3::with<error_handler_tag>(std::ref(error_handler))
        [
            boxscript::boxscript()
        ];

    // Go forth and parse!
    bool success = phrase_parse(iter, end, parser, boxscript::parser::skipper, *ast);
    if (success && iter!=end) {
        success = false;
        error_handler(iter, "Error! Expecting end of input here: ");
    }
    if (!success)
        throw base::Exception(QString::fromStdString(errorMsg.str()));
    return ast;
}

Expression parseExpression(QString s) {
    auto ast = parse("Box{&x=" + s.toStdString() + "}");
    auto root = ast->root;
    auto assignment = root.assignments.at(0);
    auto expression = boost::get<ast::Expression>(assignment.expression);
    Expression baseExpression;
    expression.build(&baseExpression);
    baseExpression.close();
    return baseExpression;
}

}}
