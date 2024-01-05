#ifndef PARSER_H
#define PARSER_H

#include <list>
#include <string>
#include <vector>

enum class token_t {
    SYMBOL,
    OPERATOR,
    LEFT_PAREN,
    RIGHT_PAREN,
};

std::list<char> lexer(std::string &input);

struct syntax_node_t {
    token_t type;
    char token;
    syntax_node_t *left;
    syntax_node_t *right;
};

class syntax_tree_t {
public:
    syntax_tree_t(std::list<char> &token_stream);
    ~syntax_tree_t();
    void visualise_tree();

private:
    syntax_node_t *root;

    syntax_node_t *construct_tree(std::vector<char> &output_stack);
    void delete_tree(syntax_node_t *node);
    void visualise(syntax_node_t *node, std::string &indent);
};

#endif // PARSER_H

