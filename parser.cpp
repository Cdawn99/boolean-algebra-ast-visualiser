#include "parser.h"
#include <cctype>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

std::map<char, std::string> operator_alias = {
    {'!', "NOT("},
    {'*', "AND("},
    {'+', "OR("},
    {'^', "XOR("},
};

token_t type_of_token(char token) {
    if (std::isalpha(token)) {
        return token_t::SYMBOL;
    }
    if (token == '(') {
        return token_t::LEFT_PAREN;
    }
    if (token == ')') {
        return token_t::RIGHT_PAREN;
    }
    return token_t::OPERATOR;
}

int compare_precedence(char operator1, char operator2) {
    if (operator1 == operator2) {
        return 0;
    }
    switch (operator1) {
    case '!':
        return 1;
    case '*':
        return operator2 == '!' ? -1 : 1;
    case '+':
    case '^':
        return operator2 == '!' || operator2 == '*' ? -1 : 0;
    }
    return 0; // Unreachable.
}

void move_operator(std::vector<char> &operator_stack, std::vector<char> &output_stack) {
    char op = operator_stack.back();
    output_stack.push_back(op);
    operator_stack.pop_back();
}

std::list<char> lexer(std::string &input) {
    std::list<char> token_stream;
    bool potential_and = false;
    for (size_t i = 0; i < input.length(); i++) {
        char token = input[i];
        if (token == ' ') {
            continue;
        }
        if (potential_and && (std::isalpha(token) || token == '!' || token == '(')) {
            token_stream.push_back('*');
            potential_and = false;
        }
        token_stream.push_back(token);
        potential_and = std::isalpha(token) || token == ')';
   }
    return token_stream;
}

// Implementation of the Shunting yard algorighm:
// https://en.wikipedia.org/wiki/Shunting_yard_algorithm
std::vector<char> parser(std::list<char> &token_stream) {
    std::vector<char> output_stack;
    std::vector<char> operator_stack;
    while (!token_stream.empty()) {
        char token = token_stream.front();
        token_t type = type_of_token(token);
        switch (type) {
        case token_t::SYMBOL:
            output_stack.push_back(token);
            break;
        case token_t::OPERATOR:
            while (!operator_stack.empty() && operator_stack.back() != '(' && compare_precedence(token, operator_stack.back()) <= 0) {
                move_operator(operator_stack, output_stack);
            }
            operator_stack.push_back(token);
            break;
        case token_t::LEFT_PAREN:
            operator_stack.push_back(token);
            break;
        case token_t::RIGHT_PAREN:
            while (operator_stack.back() != '(') {
                move_operator(operator_stack, output_stack);
            }
            operator_stack.pop_back();
            if (!operator_stack.empty() && operator_stack.back() == '!') {
                move_operator(operator_stack, output_stack);
            }
        }
        token_stream.pop_front();
    }
    while (!operator_stack.empty()) {
        move_operator(operator_stack, output_stack);
    }
    return output_stack;
}

syntax_node_t *syntax_tree_t::construct_tree(std::vector<char> &output_stack) {
    syntax_node_t *node = new syntax_node_t;
    char token = output_stack.back();
    node->token = token;
    node->type = type_of_token(token);
    output_stack.pop_back();
    if (node->type == token_t::OPERATOR) {
        node->right = construct_tree(output_stack);
        if (node->token != '!') {
            node->left = construct_tree(output_stack);
        } else {
            node->left = nullptr;
        }
    } else {
        node->left = node->right = nullptr;
    }
    return node;
}

syntax_tree_t::syntax_tree_t(std::list<char> &token_stream) {
    std::vector<char> output_stack = parser(token_stream);
    root = construct_tree(output_stack);
}

void syntax_tree_t::delete_tree(syntax_node_t *node) {
    if (node == nullptr) {
        return;
    }
    delete_tree(node->left);
    delete_tree(node->right);
    delete node;
}

syntax_tree_t::~syntax_tree_t() {
    delete_tree(root);
    root = nullptr;
}

void syntax_tree_t::visualise(syntax_node_t *node, std::string &indent) {
    if (node == nullptr) {
        return;
    }
    if (node->type == token_t::SYMBOL) {
        std::cout << indent << node->token << "\n";
        return;
    }
    std::cout << indent << operator_alias[node->token] << "\n";
    indent += "|   ";
    visualise(node->left, indent);
    visualise(node->right, indent);
    indent.resize(indent.length() - 4);
    std::cout << indent << ")\n";
}

void syntax_tree_t::visualise_tree() {
    std::string indent;
    visualise(root, indent);
}

