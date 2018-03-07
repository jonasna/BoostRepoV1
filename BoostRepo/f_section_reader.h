#pragma once
#include <fstream>
#include <vector>

class f_section_reader
{
public:

	f_section_reader(const std::string& file_name, unsigned long section_size);
	~f_section_reader();

	std::vector<char> get_next_section();

	bool get_is_valid() const;
	unsigned long get_section_size() const;
	unsigned long get_file_size() const;;
	unsigned long get_remaining_bytes() const;

private:

	std::ifstream			file_stream_;

	const unsigned long		section_size_;
	unsigned long			total_file_size_;
	unsigned long			remaining_bytes_;
	bool					is_valid_;
	
};
