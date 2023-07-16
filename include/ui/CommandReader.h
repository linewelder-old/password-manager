#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include "ui/exceptions/UnexpectedCharacterException.h"
#include "ui/exceptions/UnexpectedWordException.h"

enum class TokenType {
	WORD, STRING_LITERAL, NUMBER,
	UNEXPECTED_CHARACTER, END_OF_LINE
};

struct Token {
	TokenType type;
	std::string_view text;
	unsigned int number;
};

class CommandReader
{
	std::istream& stream;
	std::string line;

	const char* start;
	const char* current;
	Token current_token;

public:
	CommandReader(std::istream& stream);

	void new_line();
	std::string_view read_next_line();

	std::string_view read_word();
	bool can_read_word();

	void read_keyword(std::string_view expected);
	std::string_view read_string_literal();
	unsigned int read_number();
	void read_end_of_line();

private:
	Token peek_token() const;
	Token next_token();

	void skip_spaces();
	void update_current_token(TokenType type);

	void consume_word();
	void consume_string_literal();
	unsigned int consume_number();

	char peek() const;
	char advance();
};
