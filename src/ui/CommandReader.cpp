#include "ui/CommandReader.h"

#include <istream>
#include <string>
#include <string_view>

CommandReader::CommandReader(std::istream& stream)
	: stream(stream) {}

void CommandReader::new_line()
{
	getline(stream, line);
	start = &*line.begin();
	current = start;

	next_token();
}

std::string CommandReader::read_next_line()
{
	getline(stream, line);
	return line;
}

std::string CommandReader::read_word()
{
	if (!can_read_word())
	{
		throw UnexpectdCharacterException(
			ExpectedCharacterType::LETTER, peek_token().text.at(0));
	}

	return std::string(next_token().text);
}

bool CommandReader::can_read_word()
{
	return peek_token().type == TokenType::WORD;
}

void CommandReader::read_keyword(std::string expected)
{
	const std::string found_word = read_word();
	if (found_word != expected)
	{
		throw UnexpectedWordException(expected, found_word);
	}
}

std::string CommandReader::read_string_literal()
{
	if (peek_token().type != TokenType::STRING_LITERAL)
	{
		throw UnexpectdCharacterException(
			ExpectedCharacterType::QUOTE, peek_token().text.at(0));
	}

	std::string_view with_quotes = next_token().text;
	return std::string( with_quotes.substr(1, with_quotes.size() - 2) );
}

unsigned int CommandReader::read_number()
{
	if (peek_token().type != TokenType::NUMBER)
	{
		throw UnexpectdCharacterException(
			ExpectedCharacterType::DIGIT, peek_token().text.at(0));
	}

	return next_token().number;
}

void CommandReader::read_end_of_line()
{
	if (peek_token().type != TokenType::END_OF_LINE)
	{
		throw UnexpectdCharacterException(
			ExpectedCharacterType::END_OF_LINE, peek_token().text.at(0));
	}
}

Token CommandReader::peek_token() const
{
	return current_token;
}

Token CommandReader::next_token()
{
	skip_spaces();
	const Token previous_token = current_token;
	start = current;

	const char ch = peek();
	if (isalpha(ch))
	{
		consume_word();
		update_current_token(TokenType::WORD);
	}
	else if (ch == '\'')
	{
		consume_string_literal();
		update_current_token(TokenType::STRING_LITERAL);
	}
	else if (isdigit(ch))
	{
		unsigned int number = consume_number();
		update_current_token(TokenType::NUMBER);
		current_token.number = number;
	}
	else if (ch == '\0')
	{
		current_token = Token {
			.type = TokenType::END_OF_LINE,
			.text = std::string_view(current, 1)
		};
	}
	else
	{
		advance();
		update_current_token(TokenType::UNEXPECTED_CHARACTER);
	}

	return previous_token;
}

void CommandReader::skip_spaces()
{
	while (peek() == ' ')
	{
		advance();
	}
}

void CommandReader::update_current_token(TokenType type)
{
	current_token = Token {
		.type = type,
		.text = std::string_view(
			start, std::distance(start, current)),
	};
}

void CommandReader::consume_word()
{
	while ( isalpha(peek()) )
	{
		advance();
	}
}

void CommandReader::consume_string_literal()
{
	advance();
	while (true)
	{
		const char ch = advance();
		if (ch == '\'')
		{
			break;
		}
		else if (ch == '\0' || ch == '\n' || ch == '\r')
		{
			throw UnexpectdCharacterException(ExpectedCharacterType::QUOTE, ch);
		}
	}
}

unsigned int CommandReader::consume_number()
{
	unsigned int value = 0;
	while ( isdigit(peek()) )
	{
		value = 10 * value + (advance() - '0');
	}

	return value;
}

char CommandReader::peek() const
{
	return *current;
}

char CommandReader::advance()
{
	if (!*current) return '\0';
	return *current++;
}
