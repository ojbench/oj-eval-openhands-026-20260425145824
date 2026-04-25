#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <iostream>
#include <string>
#include <memory>
#include <cctype>
#include <stdexcept>
#include <any>
#include <sstream>

using namespace std;

// Forward declarations
class expr;
class number_expr;
class binary_expr;

// Visitor base class
class visitor {
public:
    virtual ~visitor() = default;
    virtual any visit(number_expr* node) = 0;
    virtual any visit(binary_expr* node) = 0;
};

// Expression base class
class expr {
public:
    virtual ~expr() = default;
    virtual any accept(visitor* v) = 0;
};

// Number expression
class number_expr : public expr {
public:
    double value;
    explicit number_expr(double v) : value(v) {}
    
    any accept(visitor* v) override {
        return v->visit(this);
    }
};

// Binary operation expression
class binary_expr : public expr {
public:
    char op;
    shared_ptr<expr> left;
    shared_ptr<expr> right;
    
    binary_expr(char o, shared_ptr<expr> l, shared_ptr<expr> r)
        : op(o), left(l), right(r) {}
    
    any accept(visitor* v) override {
        return v->visit(this);
    }
};

// Calculator visitor
class calculator : public visitor {
public:
    any visit(number_expr* node) override {
        return node->value;
    }
    
    any visit(binary_expr* node) override {
        double left_val = any_cast<double>(node->left->accept(this));
        double right_val = any_cast<double>(node->right->accept(this));
        
        switch (node->op) {
            case '+':
                return left_val + right_val;
            case '-':
                return left_val - right_val;
            case '*':
                return left_val * right_val;
            case '/':
                return left_val / right_val;
            default:
                return 0.0;
        }
    }
};

// Parser class
class expr_parser {
private:
    string input;
    size_t pos;
    
    void skip_whitespace() {
        while (pos < input.length() && isspace(input[pos])) {
            pos++;
        }
    }
    
    char peek() {
        skip_whitespace();
        if (pos < input.length()) {
            return input[pos];
        }
        return '\0';
    }
    
    char consume() {
        skip_whitespace();
        if (pos < input.length()) {
            return input[pos++];
        }
        return '\0';
    }
    
    shared_ptr<expr> parse_number() {
        skip_whitespace();
        size_t start = pos;
        
        if (pos < input.length() && (input[pos] == '-' || input[pos] == '+')) {
            pos++;
        }
        
        bool has_dot = false;
        while (pos < input.length() && (isdigit(input[pos]) || input[pos] == '.')) {
            if (input[pos] == '.') {
                if (has_dot) break;
                has_dot = true;
            }
            pos++;
        }
        
        double value = stod(input.substr(start, pos - start));
        return make_shared<number_expr>(value);
    }
    
    shared_ptr<expr> parse_primary() {
        char c = peek();
        
        if (c == '(') {
            consume();
            auto result = parse_expression();
            consume(); // consume ')'
            return result;
        }
        
        return parse_number();
    }
    
    shared_ptr<expr> parse_term() {
        auto left = parse_primary();
        
        while (true) {
            char c = peek();
            if (c == '*' || c == '/') {
                consume();
                auto right = parse_primary();
                left = make_shared<binary_expr>(c, left, right);
            } else {
                break;
            }
        }
        
        return left;
    }
    
    shared_ptr<expr> parse_expression() {
        auto left = parse_term();
        
        while (true) {
            char c = peek();
            if (c == '+' || c == '-') {
                consume();
                auto right = parse_term();
                left = make_shared<binary_expr>(c, left, right);
            } else {
                break;
            }
        }
        
        return left;
    }
    
public:
    explicit expr_parser(const string& s) : input(s), pos(0) {}
    
    shared_ptr<expr> parse() {
        return parse_expression();
    }
};

#endif // CALCULATOR_HPP
