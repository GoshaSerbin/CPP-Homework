// Copyright 2023 Gosha Serbin

#include "Exceptions.hpp"
#include "ICalculatable.hpp"
#include "Parser.hpp"

#include <cstring>
#include <memory>
#include <unordered_map>

Parser::Parser(std::string expression) : expression(expression), currentParsingPosition(0) {
    if (!hasCorrectBracketSequence()) {
        throw Error("Expression doesn't have correct bracket sequence");
    }
}

bool Parser::hasCorrectBracketSequence() noexcept {
    int bracketCounter = 0;
    for (int i = 0; i < expression.size(); ++i) {
        if (expression[i] == '(') {
            bracketCounter++;
        }
        if (expression[i] == ')') {
            bracketCounter--;
            if (bracketCounter < 0) {
                return false;
            }
        }
    }
    return bracketCounter == 0;
}

std::string Parser::parseToken() {
    currentParsingPosition = expression.find_first_not_of(" ", currentParsingPosition);
    if (currentParsingPosition >= expression.size()) {
        currentParsingPosition = expression.size();
        return emptyToken;
    }
    if (std::isdigit(expression[currentParsingPosition])) {
        size_t newPos = expression.find_first_not_of(acceptableSymbolsForNumbers, currentParsingPosition + 1);
        std::string number(expression.substr(currentParsingPosition, newPos - currentParsingPosition));
        currentParsingPosition = newPos;
        return number;
    }
    for (const auto& token : Parser::acceptableTokens) {
        if (std::strncmp(expression.c_str() + currentParsingPosition, token.c_str(), token.size()) == 0) {
            currentParsingPosition += token.size();
            return token;
        }
    }
    std::string undefinedSymbol(1, expression[currentParsingPosition]);
    std::string errorMessage = "Undefined symbol '" + undefinedSymbol + "' at position " +
                               std::to_string(currentParsingPosition) +
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
        throw Error("Can't find needed argument at position " + std::to_string(currentParsingPosition));
    }
    if (std::isdigit(token[0])) {
        calculatableObjectUPtr = std::make_unique<Number>(std::stod(token));
        return calculatableObjectUPtr;
    }

    if (token == "(") {
        if (expression[currentParsingPosition] == ')') {
            throw Error("Empty argument in brackets at position " + std::to_string(currentParsingPosition));
        };
        calculatableObjectUPtr = parse();
        std::string nextToken = parseToken();

        if (nextToken != ")") {
            std::string errorMessage {"Expected ')' at position " + std::to_string(currentParsingPosition)};
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

    std::string errorMessage {"Missing argument at position " + std::to_string(currentParsingPosition - 1)};
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
            currentParsingPosition -= operation.size();
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
