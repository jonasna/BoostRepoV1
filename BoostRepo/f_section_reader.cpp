#include "f_section_reader.h"
#include <filesystem>

using namespace std;
using namespace std::experimental::filesystem;

f_section_reader::f_section_reader(const std::string& file_name, unsigned long section_size)
	: section_size_(section_size)
{
	const path file_path(file_name);
	
	if (exists(file_path))
	{
		total_file_size_ = remaining_bytes_ = file_size(file_path);
		file_stream_.open(file_name, ios::binary);
		if (file_stream_)
			is_valid_ = true;
	}
	else
	{
		total_file_size_ = remaining_bytes_ = 0;
		is_valid_ = false;
	}
		
}

f_section_reader::~f_section_reader()
{
	if (is_valid_)
		file_stream_.close();
}

std::vector<char> f_section_reader::get_next_section()
{
	const auto buf_size = std::min(section_size_, remaining_bytes_);
	std::vector<char> buf(buf_size);

	if(buf_size)
	{
		file_stream_.read(&buf[0], buf_size);
		remaining_bytes_ = remaining_bytes_ - buf_size;
	}

	return buf;
}

bool f_section_reader::get_is_valid() const
{
	return is_valid_;
}

unsigned long f_section_reader::get_section_size() const
{
	return section_size_;
}

unsigned long f_section_reader::get_file_size() const
{
	return total_file_size_;
}

unsigned long f_section_reader::get_remaining_bytes() const
{
	return remaining_bytes_;
}
