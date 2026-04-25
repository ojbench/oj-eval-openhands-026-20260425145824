#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <any>

// Forward declarations
struct visitor;
struct num_node;
struct add_node;
struct sub_node;
struct mul_node;
struct div_node;

// Calculator visitor implementation
struct calculator : visitor {
    std::any visit_num(num_node *node) override;
    std::any visit_add(add_node *node) override;
    std::any visit_sub(sub_node *node) override;
    std::any visit_mul(mul_node *node) override;
    std::any visit_div(div_node *node) override;
};

// Implementation (to be included after visitor.h is included)
std::any calculator::visit_num(num_node *node) {
    return node->value;
}

std::any calculator::visit_add(add_node *node) {
    auto left_val = std::any_cast<long long>(node->lhs->accept(this));
    auto right_val = std::any_cast<long long>(node->rhs->accept(this));
    return left_val + right_val;
}

std::any calculator::visit_sub(sub_node *node) {
    auto left_val = std::any_cast<long long>(node->lhs->accept(this));
    auto right_val = std::any_cast<long long>(node->rhs->accept(this));
    return left_val - right_val;
}

std::any calculator::visit_mul(mul_node *node) {
    auto left_val = std::any_cast<long long>(node->lhs->accept(this));
    auto right_val = std::any_cast<long long>(node->rhs->accept(this));
    return left_val * right_val;
}

std::any calculator::visit_div(div_node *node) {
    auto left_val = std::any_cast<long long>(node->lhs->accept(this));
    auto right_val = std::any_cast<long long>(node->rhs->accept(this));
    return left_val / right_val;
}

#endif // CALCULATOR_HPP
