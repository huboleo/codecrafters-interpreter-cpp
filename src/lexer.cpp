#include "lexer.hpp"
#include <expected>
#include <format>
#include <print>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace {
std::string literal_to_string(const std::variant<std::monostate, double, std::string> literal) {
    if (std::holds_alternative<std::monostate>(literal)) {
        return "null";
    }

    if (std::holds_alternative<double>(literal)) {
        std::string text = std::format("{}", std::get<double>(literal));

        if (text.find_first_of(".eE") == std::string::npos) {
            text += ".0";
        }

        return text;
    }

    return std::get<std::string>(literal);
}

bool is_digit(char character) { return character >= '0' && character <= '9'; }

bool is_alpha(char character) {
    return (character >= 'a' && character <= 'z') || (character >= 'A' && character <= 'Z') ||
           character == '_';
}

bool is_alpha_numeric(char character) { return is_digit(character) || is_alpha(character); }

} // namespace

std::string_view token_type_to_string(TokenType token_type) {
    switch (token_type) {
    case TokenType::LEFT_PAREN:
        return "LEFT_PAREN";
    case TokenType::RIGHT_PAREN:
        return "RIGHT_PAREN";
    case TokenType::LEFT_BRACE:
        return "LEFT_BRACE";
    case TokenType::RIGHT_BRACE:
        return "RIGHT_BRACE";
    case TokenType::COMMA:
        return "COMMA";
    case TokenType::DOT:
        return "DOT";
    case TokenType::MINUS:
        return "MINUS";
    case TokenType::PLUS:
        return "PLUS";
    case TokenType::SEMICOLON:
        return "SEMICOLON";
    case TokenType::SLASH:
        return "SLASH";
    case TokenType::STAR:
        return "STAR";
    case TokenType::BANG:
        return "BANG";
    case TokenType::BANG_EQUAL:
        return "BANG_EQUAL";
    case TokenType::EQUAL:
        return "EQUAL";
    case TokenType::EQUAL_EQUAL:
        return "EQUAL_EQUAL";
    case TokenType::GREATER:
        return "GREATER";
    case TokenType::GREATER_EQUAL:
        return "GREATER_EQUAL";
    case TokenType::LESS:
        return "LESS";
    case TokenType::LESS_EQUAL:
        return "LESS_EQUAL";
    case TokenType::IDENTIFIER:
        return "IDENTIFIER";
    case TokenType::STRING:
        return "STRING";
    case TokenType::NUMBER:
        return "NUMBER";
    case TokenType::AND:
        return "AND";
    case TokenType::CLASS:
        return "CLASS";
    case TokenType::ELSE:
        return "ELSE";
    case TokenType::FALSE:
        return "FALSE";
    case TokenType::FUN:
        return "FUN";
    case TokenType::FOR:
        return "FOR";
    case TokenType::IF:
        return "IF";
    case TokenType::NIL:
        return "NIL";
    case TokenType::OR:
        return "OR";
    case TokenType::PRINT:
        return "PRINT";
    case TokenType::RETURN:
        return "RETURN";
    case TokenType::SUPER:
        return "SUPER";
    case TokenType::THIS:
        return "THIS";
    case TokenType::TRUE:
        return "TRUE";
    case TokenType::VAR:
        return "VAR";
    case TokenType::WHILE:
        return "WHILE";
    case TokenType::EOF_TOKEN:
        return "EOF";
    }

    return "UNKNOWN";
}

Lexer::Lexer(std::string source) : _source_file_content(std::move(source)) {}

char Lexer::advance() { return _source_file_content[_current++]; }

char Lexer::peek() {
    if (is_at_end()) {
        return '\0';
    }
    return _source_file_content[_current];
}

char Lexer::peek_next() {
    if (_current + 1 >= _source_file_content.size()) {
        return '\0';
    }
    return _source_file_content[_current + 1];
}

void Lexer::parse_string() {
    while (peek() != '"' && !is_at_end()) {
        if (peek() == '\n') {
            _line++;
        }
        advance();
    }

    if (is_at_end()) {
        report_error("Unterminated string.");
        return;
    }

    // For closing "
    advance();

    auto value = _source_file_content.substr(_start + 1, _current - _start - 2);
    add_token(TokenType::STRING, value);
}

void Lexer::parse_number() {
    while (is_digit(peek())) {
        advance();
    }

    if (peek() == '.' && is_digit(peek_next())) {
        advance();

        while (is_digit(peek())) {
            advance();
        }
    }

    double value = std::stod(_source_file_content.substr(_start, _current - _start));
    add_token(TokenType::NUMBER, value);
}

void Lexer::parse_identifier() {
    while (is_alpha_numeric(peek())) {
        advance();
    }

    auto value = _source_file_content.substr(_start, _current - _start);

    if (keywords.contains(value)) {
        add_token(keywords.at(value));
    } else {
        add_token(TokenType::IDENTIFIER);
    }
}

void Lexer::add_token(TokenType type,
                      const std::variant<std::monostate, double, std::string>& literal) {
    auto lexeme =
        type == TokenType::EOF_TOKEN ? "" : _source_file_content.substr(_start, _current - _start);
    std::println("{} {} {}", token_type_to_string(type), lexeme, literal_to_string(literal));
    _tokens.push_back(Token{
        .type = type,
        .lexeme = lexeme,
        .literal = literal,
        .line = _line,
    });
}

bool Lexer::is_at_end() { return _current >= _source_file_content.size(); }

bool Lexer::is_match(char expected) {
    if (is_at_end()) {
        return false;
    }

    if (_source_file_content[_current] != expected) {
        return false;
    }

    _current++;
    return true;
}

void Lexer::add_token(TokenType type) { add_token(type, std::monostate{}); }

void Lexer::report_error(const std::string& cause) {
    std::println(stderr, "[line {}] Error: {}", _line, cause);
    _has_error = true;
}

void Lexer::scan_token() {
    char current_char = advance();

    switch (current_char) {
    case '(':
        add_token(TokenType::LEFT_PAREN);
        break;
    case ')':
        add_token(TokenType::RIGHT_PAREN);
        break;
    case '{':
        add_token(TokenType::LEFT_BRACE);
        break;
    case '}':
        add_token(TokenType::RIGHT_BRACE);
        break;
    case ',':
        add_token(TokenType::COMMA);
        break;
    case '.':
        add_token(TokenType::DOT);
        break;
    case '-':
        add_token(TokenType::MINUS);
        break;
    case '+':
        add_token(TokenType::PLUS);
        break;
    case '*':
        add_token(TokenType::STAR);
        break;
    case ';':
        add_token(TokenType::SEMICOLON);
        break;
    case '!':
        add_token(is_match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
        break;
    case '=':
        add_token(is_match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
        break;
    case '<':
        add_token(is_match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
        break;
    case '>':
        add_token(is_match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
        break;
    case '/':
        if (is_match('/')) {
            while (peek() != '\n' && !is_at_end()) {
                advance();
            }
        } else {
            add_token(TokenType::SLASH);
        }
        break;
    case ' ':
    case '\r':
    case '\t':
        break;
    case '\n':
        _line++;
        break;
    case '"': {
        parse_string();
        break;
    }
    default:
        if (is_digit(current_char)) {
            parse_number();
        } else if (is_alpha(current_char)) {
            parse_identifier();
        } else {
            report_error(std::format("Unexpected character: {}", current_char));
        }
        break;
    }
}

std::expected<std::vector<Token>, std::string> Lexer::tokenize() {

    while (!is_at_end()) {
        _start = _current;
        scan_token();
    }
    add_token(TokenType::EOF_TOKEN);

    if (_has_error) {
        return std::unexpected<std::string>("Syntax error occured");
    }

    return _tokens;
}
