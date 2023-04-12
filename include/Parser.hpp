#pragma once

#include "ICalculatable.hpp"

#include <string>
#include <vector>
#include <unordered_map>

namespace BinaryOperations {
const std::string add = "+";
const std::string substract = "-";
const std::string divide = "/";
};  // namespace BinaryOperations

using OperationPriority = int;

class Parser {
 public:
    explicit Parser(std::string expression);
    [[nodiscard]] ICalculatableUPtr parse();
    static std::string printAcceptableTokens();

    inline static const std::vector<std::string> acceptableTokens {"+", "-", "/", "abs", "atan", "(", ")"};
    inline static const std::string acceptableSymbolsForNumbers {".0123456789"};
    inline static const std::string emptyToken {""};
    inline static const std::unordered_map<std::string, OperationPriority> priorityByOperation {
        {BinaryOperations::add, 1}, {BinaryOperations::substract, 1}, {BinaryOperations::divide, 2}};

 private:
    [[nodiscard]] bool hasCorrectBracketSequence() noexcept;
    OperationPriority getPriority(const std::string& operation) noexcept;
    [[nodiscard]] std::string parseToken();
    [[nodiscard]] ICalculatableUPtr parseSimpleCalculatableObject();
    [[nodiscard]] ICalculatableUPtr parseBinaryCalculatableObject(OperationPriority currentPriority);

    std::string m_expression;
    size_t m_currentParsingPosition;
};
