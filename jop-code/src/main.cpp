//
// Created by Joseph on 26/09/2018.
//

#include <antlr4-runtime.h>
#include "TLexer.h"
#include "TParser.h"

using namespace antlr4;
using namespace T;

int main() {
    ANTLRInputStream x;
    ANTLRInputStream input(u8"🍴 = 🍐 + \"😎\";(((x * π))) * µ + ∰; a + (x * (y ? 0 : 1) + z);");
    TLexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    tokens.fill();

    TParser parser(&tokens);
    tree::ParseTree* tree = parser.main();

    std::cout << tree->toStringTree(&parser) << std::endl << std::endl;

    return 0;
}

