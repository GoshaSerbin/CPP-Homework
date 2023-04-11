#pragma once

#include <cmath>
#include <memory>

class ICalculatable {
 public:
    virtual double Calculate() = 0;
    virtual ~ICalculatable() {};
};

using ICalculatableUPtr = std::unique_ptr<ICalculatable>;

class Number : public ICalculatable {
 public:
    explicit Number(double value) : m_value(value) {};
    double Calculate() override { return m_value; }

 private:
    double m_value;
};

class UnaryOperation : public ICalculatable {
 public:
    explicit UnaryOperation(ICalculatableUPtr&& argument) : m_argument(std::move(argument)) {};
    virtual double Calculate() = 0;

 protected:
    ICalculatableUPtr m_argument;
};

class Atan : public UnaryOperation {
 public:
    explicit Atan(ICalculatableUPtr&& argument) : UnaryOperation(std::move(argument)) {}
    double Calculate() override { return std::atan(m_argument->Calculate()); }
};

class Minus : public UnaryOperation {
 public:
    explicit Minus(ICalculatableUPtr&& argument) : UnaryOperation(std::move(argument)) {}
    double Calculate() override { return -m_argument->Calculate(); }
};

class Abs : public UnaryOperation {
 public:
    explicit Abs(ICalculatableUPtr&& argument) : UnaryOperation(std::move(argument)) {}
    double Calculate() override { return std::abs(m_argument->Calculate()); }
};

class BinaryOperation : public ICalculatable {
 public:
    explicit BinaryOperation(ICalculatableUPtr&& left, ICalculatableUPtr&& right)
        : m_left(std::move(left)), m_right(std::move(right)) {};
    virtual double Calculate() = 0;

 protected:
    ICalculatableUPtr m_left;
    ICalculatableUPtr m_right;
};

class Add : public BinaryOperation {
 public:
    explicit Add(ICalculatableUPtr&& left, ICalculatableUPtr&& right)
        : BinaryOperation(std::move(left), std::move(right)) {}
    double Calculate() override { return m_left->Calculate() + m_right->Calculate(); }
};

class Subtract : public BinaryOperation {
 public:
    explicit Subtract(ICalculatableUPtr&& left, ICalculatableUPtr&& right)
        : BinaryOperation(std::move(left), std::move(right)) {}
    double Calculate() { return m_left->Calculate() - m_right->Calculate(); }
};

class Divide : public BinaryOperation {
 public:
    explicit Divide(ICalculatableUPtr&& left, ICalculatableUPtr&& right)
        : BinaryOperation(std::move(left), std::move(right)) {}
    double Calculate() { return m_left->Calculate() / m_right->Calculate(); }
};
