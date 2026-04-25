#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <any>

// Calculator visitor implementation
// Assumes the test framework provides:
// - visitor base class with virtual methods
// - num_node, add_node, sub_node, mul_node, div_node classes

struct calculator : visitor {
    std::any visit_num(num_node *node) override {
        return node->value;
    }
    
    std::any visit_add(add_node *node) override {
        auto left_val = std::any_cast<long long>(node->lhs->accept(this));
        auto right_val = std::any_cast<long long>(node->rhs->accept(this));
        return left_val + right_val;
    }
    
    std::any visit_sub(sub_node *node) override {
        auto left_val = std::any_cast<long long>(node->lhs->accept(this));
        auto right_val = std::any_cast<long long>(node->rhs->accept(this));
        return left_val - right_val;
    }
    
    std::any visit_mul(mul_node *node) override {
        auto left_val = std::any_cast<long long>(node->lhs->accept(this));
        auto right_val = std::any_cast<long long>(node->rhs->accept(this));
        return left_val * right_val;
    }
    
    std::any visit_div(div_node *node) override {
        auto left_val = std::any_cast<long long>(node->lhs->accept(this));
        auto right_val = std::any_cast<long long>(node->rhs->accept(this));
        return left_val / right_val;
    }
};

#endif // CALCULATOR_HPP
