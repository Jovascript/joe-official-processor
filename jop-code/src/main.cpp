//
// Created by Joseph on 26/09/2018.
//

#include <antlr4-runtime.h>
#include "JScriptLexer.h"
#include "JScriptParser.h"


using namespace antlr4;
using namespace JScript;

int main() {
    ANTLRInputStream x;
    ANTLRInputStream input(R"(
int z()

void x(int y,
string l) {
    bool m = y/7
}
y = 6
str = "hello"
int ll = x(2,3)
   )");
    JScriptLexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    tokens.fill();

    JScriptParser parser(&tokens);
    tree::ParseTree* tree = parser.main();

    std::cout << tree->toStringTree(&parser) << std::endl << std::endl;
    return 0;
}

