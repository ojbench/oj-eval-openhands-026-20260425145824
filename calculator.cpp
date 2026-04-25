#include <any>

struct calculator {
    std::any visit_num(auto *node) {
        return node->value;
    }
    
    std::any visit_add(auto *node) {
        auto left_val = std::any_cast<long long>(node->lhs->accept(this));
        auto right_val = std::any_cast<long long>(node->rhs->accept(this));
        return left_val + right_val;
    }
    
    std::any visit_sub(auto *node) {
        auto left_val = std::any_cast<long long>(node->lhs->accept(this));
        auto right_val = std::any_cast<long long>(node->rhs->accept(this));
        return left_val - right_val;
    }
    
    std::any visit_mul(auto *node) {
        auto left_val = std::any_cast<long long>(node->lhs->accept(this));
        auto right_val = std::any_cast<long long>(node->rhs->accept(this));
        return left_val * right_val;
    }
    
    std::any visit_div(auto *node) {
        auto left_val = std::any_cast<long long>(node->lhs->accept(this));
        auto right_val = std::any_cast<long long>(node->rhs->accept(this));
        return left_val / right_val;
    }
};

