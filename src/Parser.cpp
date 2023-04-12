// Copyright 2023 Gosha Serbin

#include "Exceptions.hpp"
#include "ICalculatable.hpp"
#include "Parser.hpp"

#include <cstring>
#include <memory>
#include <unordered_map>

Parser::Parser(std::string expression) : m_expression(expression), m_currentParsingPosition(0) {
    if (!hasCorrectBracketSequence()) {
        throw Error("Expression doesn't have correct bracket sequence");
    }
}

bool Parser::hasCorrectBracketSequence() noexcept {
    int bracketCounter = 0;
    for (int i = 0; i < m_expression.size(); ++i) {
        if (m_expression[i] == '(') {
            bracketCounter++;
        }
        if (m_expression[i] == ')') {
            bracketCounter--;
            if (bracketCounter < 0) {
                return false;
            }
        }
    }
    return bracketCounter == 0;
}

std::string Parser::parseToken() {
    m_currentParsingPosition = m_expression.find_first_not_of(" ", m_currentParsingPosition);
    if (m_currentParsingPosition >= m_expression.size()) {
        m_currentParsingPosition = m_expression.size();
        return emptyToken;
    }
    if (std::isdigit(m_expression[m_currentParsingPosition])) {
        size_t newPos =
            m_expression.find_first_not_of(acceptableSymbolsForNumbers, m_currentParsingPosition + 1);
        std::string number(m_expression.substr(m_currentParsingPosition, newPos - m_currentParsingPosition));
        m_currentParsingPosition = newPos;
        return number;
    }
    for (const auto& token : Parser::acceptableTokens) {
        if (std::strncmp(m_expression.c_str() + m_currentParsingPosition, token.c_str(), token.size()) == 0) {
            m_currentParsingPosition += token.size();
            return token;
        }
    }
    std::string undefinedSymbol(1, m_expression[m_currentParsingPosition]);
    std::string errorMessage = "Undefined symbol '" + undefinedSymbol + "' at position " +
                               std::to_string(m_currentParsingPosition) +
                               ", acceptable tokens: " + Parser::printAcceptableTokens();
    throw Error(errorMessage);
}

std::string Parser::printAcceptableTokens() {
    std::string out {};
    for (const auto token : acceptableTokens) {
        out += "'" + token + "' ";
    }
    return out;
}

ICalculatableUPtr Parser::parseSimpleCalculatableObject() {
    ICalculatableUPtr calculatableObjectUPtr;

    std::string token = parseToken();
    if (token == emptyToken) {
        throw Error("Can't find needed argument at position " + std::to_string(m_currentParsingPosition));
    }
    if (std::isdigit(token[0])) {
        calculatableObjectUPtr = std::make_unique<Number>(std::stod(token));
        return calculatableObjectUPtr;
    }

    if (token == "(") {
        if (m_expression[m_currentParsingPosition] == ')') {
            throw Error("Empty argument in brackets at position " + std::to_string(m_currentParsingPosition));
        };
        calculatableObjectUPtr = parse();
        std::string nextToken = parseToken();

        if (nextToken != ")") {
            std::string errorMessage {"Expected ')' at position " + std::to_string(m_currentParsingPosition)};
            throw Error(errorMessage);
        }

        return calculatableObjectUPtr;
    }

    if (token == "atan") {
        calculatableObjectUPtr = std::make_unique<Atan>(std::move(parseSimpleCalculatableObject()));
        return calculatableObjectUPtr;
    }
    if (token == "abs") {
        calculatableObjectUPtr = std::make_unique<Abs>(std::move(parseSimpleCalculatableObject()));
        return calculatableObjectUPtr;
    }
    if (token == "-") {
        calculatableObjectUPtr = std::make_unique<Minus>(std::move(parseSimpleCalculatableObject()));
        return calculatableObjectUPtr;
    }

    std::string errorMessage {"Missing argument at position " + std::to_string(m_currentParsingPosition - 1)};
    throw Error(errorMessage);
}

OperationPriority Parser::getPriority(const std::string& operation) noexcept {
    if (priorityByOperation.find(operation) != priorityByOperation.end()) {
        return priorityByOperation.at(operation);
    } else {
        return 0;
    }
}

ICalculatableUPtr Parser::parseBinaryCalculatableObject(OperationPriority currentPriority) {
    ICalculatableUPtr leftCalculatableObject = parseSimpleCalculatableObject();

    while (true) {
        std::string operation = parseToken();
        if (operation == emptyToken) {
            return leftCalculatableObject;
        }
        OperationPriority newPriority = getPriority(operation);
        if (newPriority <= currentPriority) {
            m_currentParsingPosition -= operation.size();
            return leftCalculatableObject;
        }

        ICalculatableUPtr rightCalculatableObject = parseBinaryCalculatableObject(newPriority);
        if (operation == BinaryOperations::add) {
            leftCalculatableObject =
                std::make_unique<Add>(std::move(leftCalculatableObject), std::move(rightCalculatableObject));
        }
        if (operation == BinaryOperations::substract) {
            leftCalculatableObject = std::make_unique<Subtract>(std::move(leftCalculatableObject),
                                                                std::move(rightCalculatableObject));
        }
        if (operation == BinaryOperations::divide) {
            leftCalculatableObject = std::make_unique<Divide>(std::move(leftCalculatableObject),
                                                              std::move(rightCalculatableObject));
        }
    }
}

ICalculatableUPtr Parser::parse() {
    OperationPriority currentPriority = 0;
    return parseBinaryCalculatableObject(currentPriority);
}
