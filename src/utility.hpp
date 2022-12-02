#pragma once

#include <filesystem>
#include <fstream>
#include <vector>

template <class T>
std::vector<T> cache_input()
{
	std::ifstream in{ std::filesystem::path{ INPUT_DIR } / "input.txt" };
	return std::vector<T>(std::istream_iterator<T>{ in }, std::istream_iterator<T>{});
}
