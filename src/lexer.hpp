#pragma once

#include <expected>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

enum class TokenType {
    // Single-chracter tokens
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,

    // One or two character tokens
    BANG,
    BANG_EQUAL,
    EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,

    // Literals
    IDENTIFIER,
    STRING,
    NUMBER,

    // Keywords
    AND,
    CLASS,
    ELSE,
    FALSE,
    FUN,
    FOR,
    IF,
    NIL,
    OR,
    PRINT,
    RETURN,
    SUPER,
    THIS,
    TRUE,
    VAR,
    WHILE,

    EOF_TOKEN
};

std::string_view token_type_to_string(TokenType token_type);

struct Token {
    TokenType type;
    std::string lexeme;
    std::variant<std::monostate, double, std::string> literal;
    int line;
};

class Lexer {
  public:
    explicit Lexer(std::string source);
    std::expected<std::vector<Token>, std::string> tokenize();

  private:
    std::string _source_file_content;
    std::vector<Token> _tokens;
    int _start = 0;
    int _current = 0;
    int _line = 1;
    char peek();
    char advance();
    std::string get_error_message(int line, const std::string& message);
    void add_token(TokenType type);
    void add_token(TokenType type, std::variant<std::monostate, double, std::string> literal);
    bool is_at_end();
    bool is_match(char expected);
};
