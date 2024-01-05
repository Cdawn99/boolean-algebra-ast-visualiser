#include "parser.h"
#include <cctype>
#include <iostream>
#include <list>
#include <string>
#include <vector>

void display_help() {
    std::cout << "The boolean operators are:\n"
              << "(, ) : Parentheses (0)\n"
              << "! : NOT(1)\n"
              << "* : AND(2)\n"
              << "+ : OR(3)\n"
              << "^ : XOR(3)\n"
              << "The numbers correspond to their precedence with (0) being the highest and (3) being the lowest\n"
              << "OR and XOR are evaluated from left to right\n"
              << "a + b ^ c == (a + b) ^ c and a ^ b + c == (a ^ b) + c\n"
              << "The operator AND doesn't have to be explicitly typed in\n"
              << "ab == a*b\n"
              << "To quit type quit()\n"
              << "To see the help text again, type help()\n";
}

bool verify_input(std::string user_input) {
    char state = 1;
    std::vector<char> stack = {'N'};
    for (char c : user_input) {
        if (c == ' ') continue;
        if (std::isalpha(c)) {
            state = 2;
            continue;
        }
        switch (c) {
        case '(':
            state = 1;
            stack.push_back('P');
            break;
        case ')':
            if (state != 2 || stack.back() != 'P') {
                std::cout << "[ERROR] Mismatched or empty parentheses or operator before closing parentesis!\n";
                return false;
            }
            stack.pop_back();
            break;
        case '*':
        case '+':
        case '^':
            if (state == 1) {
                std::cout << "[ERROR] Invalid operator at the start of the expression or right after a patenthesis!\n";
                return false;
            }
            [[fallthrough]];
        case '!':
            state = 1;
            break;
        default: 
            std::cout << "[ERROR] Invalid symbol in expression!\n";
            return false;
        }
    }
    if (state != 2 || stack.back() != 'N') {
        std::cout << "[ERROR] Improperly closed parentheses or operator at the end of the expression!\n";
        return false;
    }
    return true;
}

bool read_input() {
    std::string expression;
    bool valid_input = false;
    while (!valid_input) {
        std::cout << "> ";
        std::getline(std::cin, expression);
        if (expression == "") {
            continue;
        }
        if (expression == "quit()") {
            return true;
        }
        if (expression == "help()") {
            display_help();
            continue;
        }
        valid_input = verify_input(expression);
    }
    std::list<char> token_stream = lexer(expression);
    syntax_tree_t ast(token_stream);
    ast.visualise_tree();
    return false;
}

int main() {
    std::cout << "*** Boolean Algebra AST visualiser ***\n"
              << "The purpose of this program is to visualise the Abstract Syntax Tree (AST)\n"
              << "of boolean algebra expressions\n"
              << "Simply type an expression and the generated AST will be visualised\n"
              << "For example: \"ab + c + a!d ^ b*c\n"
              << "For more information type help()\n"
              << "To quit type quit()\n";
    bool quit = false;
    while (!quit) {
        quit = read_input();
    }
    return 0;
}

