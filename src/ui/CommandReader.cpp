#include "ui/CommandReader.h"

#include <string>
#include <sstream>

CommandReader::CommandReader(std::istream& stream)
	: stream(stream) {}

void CommandReader::new_line()
{
	getline(stream, line);
	line_stream = std::istringstream(line);
}

std::string CommandReader::read_next_line()
{
	getline(stream, line);
	return line;
}

std::string CommandReader::read_word()
{
	skip_spaces();

	const char peeked = (char)line_stream.peek();
	if (!isalpha(peeked))
	{
		throw UnexpectdCharacterException(ExpectedCharacterType::LETTER, peeked);
	}

	std::stringstream ss;

	while ( isalpha((char)line_stream.peek()) )
	{
		ss << (char)line_stream.get();
	}

	return ss.str();
}

bool CommandReader::can_read_word()
{
	skip_spaces();

	const char peeked = (char)line_stream.peek();
	return isalpha(peeked);
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
	skip_spaces();

	const char quote = line_stream.get();
	if (quote != '\'')
	{
		throw UnexpectdCharacterException(ExpectedCharacterType::QUOTE, quote);
	}

	std::stringstream ss;
	while (true)
	{
		const int ch = line_stream.get();
		if (ch == -1 || ch == '\n' || ch == '\r')
		{
			throw UnexpectdCharacterException(ExpectedCharacterType::QUOTE, ch);
		}
		else if (ch == '\'')
		{
			break;
		}

		ss << (char)ch;
	}

	return ss.str();
}

unsigned int CommandReader::read_number()
{
	skip_spaces();

	const char peeked = (char)line_stream.peek();
	if (!isdigit(peeked))
	{
		throw UnexpectdCharacterException(ExpectedCharacterType::DIGIT, peeked);
	}

	unsigned int value = 0;
	while (isdigit((char)line_stream.peek()))
	{
		value = 10 * value + (char)line_stream.get() - '0';
	}

	return value;
}

void CommandReader::read_end_of_line()
{
	skip_spaces();

	const char ch = line_stream.get();
	if (ch != -1)
	{
		throw UnexpectdCharacterException(ExpectedCharacterType::END_OF_LINE, ch);
	}
}

void CommandReader::skip_spaces()
{
	while ( (char)line_stream.peek() == ' ')
	{
		line_stream.get();
	}
}
