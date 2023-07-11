#pragma once

#include <iostream>
#include "data/PasswordBase.h"
#include "ui/CommandReader.h"

class CommandLine
{
	PasswordBase& base;
	CommandReader reader;
	std::vector<PasswordRecord> selected_passwords;

public:
	CommandLine(PasswordBase& base);

	void run();

private:
	void read_and_run_command();

	void read_and_run_find();
	void read_and_run_sort(
		const std::vector<SearchParameter>& search_for = std::vector<SearchParameter>());
	void read_and_run_show();
	void read_and_run_edit();
	void read_and_run_add_password();
	void read_and_run_add_category();
	void read_and_run_remove_password();
	void read_and_run_remove_category();
	void read_and_run_help();

	PasswordRecord get_selected_password(unsigned int index);

	void run_find_sort(
		const std::vector<SearchParameter>& search_for,
		const std::vector<PasswordProperty>& sort_by);
};
