# Lox Lexer

A lexer for the [Lox](https://craftinginterpreters.com/the-lox-language.html) language, written from scratch in C++23.

This project was built following codecrafters interpreter challenge and the scanning chapter of [Crafting Interpreters](https://craftinginterpreters.com/) book by Robert Nystrom.

## Features

- Recognizes Lox punctuation, arithmetic operators, and comparison operators.
- Handles one- or two-character tokens including `!=`, `==`, `<=`, and `>=`.
- Parses identifiers, all Lox keywords, number literals, and string literals.
- Supports multi-line strings and `//` comments.
- Tracks source lines and reports unexpected characters or unterminated strings.

## Testing
Project uses codecrafters test suite, with all tests passing.
