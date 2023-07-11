#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include "ui/exceptions/UnexpectedCharacterException.h"
#include "ui/exceptions/UnexpectedWordException.h"

class CommandReader
{
	std::istream& stream;
	std::string line;
	std::istringstream line_stream;

public:
	CommandReader(std::istream& stream);

	void new_line();

	std::string read_word();
	bool can_read_word();

	void read_keyword(std::string expected);
	std::string read_string_literal();
	unsigned int read_number();
	void read_end_of_line();

private:
	void skip_spaces();
};
