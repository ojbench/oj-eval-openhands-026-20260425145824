#include <iostream>
#include <string>
#include <memory>
#include <cctype>
#include <stdexcept>
#include <any>

using namespace std;

// Forward declaration
class Visitor;

// Base class for all expression nodes
class Expr {
public:
    virtual ~Expr() = default;
    virtual any accept(Visitor& visitor) = 0;
};

// Number literal node
class Number : public Expr {
public:
    double value;
    explicit Number(double v) : value(v) {}
    any accept(Visitor& visitor) override;
};

// Binary operation node
class BinaryOp : public Expr {
public:
    char op;
    unique_ptr<Expr> left;
    unique_ptr<Expr> right;
    
    BinaryOp(char o, unique_ptr<Expr> l, unique_ptr<Expr> r)
        : op(o), left(move(l)), right(move(r)) {}
    any accept(Visitor& visitor) override;
};

// Visitor interface
class Visitor {
public:
    virtual ~Visitor() = default;
    virtual any visitNumber(Number* node) = 0;
    virtual any visitBinaryOp(BinaryOp* node) = 0;
};

// Implement accept methods
any Number::accept(Visitor& visitor) {
    return visitor.visitNumber(this);
}

any BinaryOp::accept(Visitor& visitor) {
    return visitor.visitBinaryOp(this);
}

// Evaluation visitor
class EvalVisitor : public Visitor {
public:
    any visitNumber(Number* node) override {
        return node->value;
    }
    
    any visitBinaryOp(BinaryOp* node) override {
        double left_val = any_cast<double>(node->left->accept(*this));
        double right_val = any_cast<double>(node->right->accept(*this));
        
        switch (node->op) {
            case '+':
                return left_val + right_val;
            case '-':
                return left_val - right_val;
            case '*':
                return left_val * right_val;
            case '/':
                if (right_val == 0) {
                    throw runtime_error("Division by zero");
                }
                return left_val / right_val;
            default:
                throw runtime_error("Unknown operator");
        }
    }
};

// Parser class
class Parser {
private:
    string expr;
    size_t pos;
    
    void skipWhitespace() {
        while (pos < expr.length() && isspace(expr[pos])) {
            pos++;
        }
    }
    
    char peek() {
        skipWhitespace();
        if (pos < expr.length()) {
            return expr[pos];
        }
        return '\0';
    }
    
    char consume() {
        skipWhitespace();
        if (pos < expr.length()) {
            return expr[pos++];
        }
        return '\0';
    }
    
    unique_ptr<Expr> parseNumber() {
        skipWhitespace();
        size_t start = pos;
        bool has_dot = false;
        
        if (pos < expr.length() && (expr[pos] == '-' || expr[pos] == '+')) {
            pos++;
        }
        
        while (pos < expr.length() && (isdigit(expr[pos]) || expr[pos] == '.')) {
            if (expr[pos] == '.') {
                if (has_dot) break;
                has_dot = true;
            }
            pos++;
        }
        
        if (start == pos || (pos == start + 1 && (expr[start] == '-' || expr[start] == '+'))) {
            throw runtime_error("Invalid number");
        }
        
        double value = stod(expr.substr(start, pos - start));
        return make_unique<Number>(value);
    }
    
    unique_ptr<Expr> parsePrimary() {
        char c = peek();
        
        if (c == '(') {
            consume();
            auto result = parseExpression();
            if (consume() != ')') {
                throw runtime_error("Expected ')'");
            }
            return result;
        }
        
        if (isdigit(c) || c == '-' || c == '+' || c == '.') {
            return parseNumber();
        }
        
        throw runtime_error("Unexpected character");
    }
    
    unique_ptr<Expr> parseTerm() {
        auto left = parsePrimary();
        
        while (true) {
            char c = peek();
            if (c == '*' || c == '/') {
                consume();
                auto right = parsePrimary();
                left = make_unique<BinaryOp>(c, move(left), move(right));
            } else {
                break;
            }
        }
        
        return left;
    }
    
    unique_ptr<Expr> parseExpression() {
        auto left = parseTerm();
        
        while (true) {
            char c = peek();
            if (c == '+' || c == '-') {
                consume();
                auto right = parseTerm();
                left = make_unique<BinaryOp>(c, move(left), move(right));
            } else {
                break;
            }
        }
        
        return left;
    }
    
public:
    explicit Parser(const string& expression) : expr(expression), pos(0) {}
    
    unique_ptr<Expr> parse() {
        return parseExpression();
    }
};

int main() {
    string line;
    while (getline(cin, line)) {
        if (line.empty()) continue;
        
        try {
            Parser parser(line);
            auto ast = parser.parse();
            
            EvalVisitor evaluator;
            double result = any_cast<double>(ast->accept(evaluator));
            
            // Check if result is an integer
            if (result == static_cast<int>(result)) {
                cout << static_cast<int>(result) << endl;
            } else {
                cout << result << endl;
            }
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
            return 1;
        }
    }
    
    return 0;
}
