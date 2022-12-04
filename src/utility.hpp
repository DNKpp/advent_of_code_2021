#pragma once

#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <charconv>

#include <range/v3/view/facade.hpp>

template <class T>
std::vector<T> cache_input()
{
	std::ifstream in{ std::filesystem::path{ INPUT_DIR } / "input.txt" };
	return std::vector<T>(std::istream_iterator<T>{ in }, std::istream_iterator<T>{});
}

class getline_range
	: public ranges::view_facade<getline_range>
{
	friend ranges::range_access;

private:
	std::istream* m_IStream{};
	std::string m_Line{};
	char m_Delimiter{ '\n' };

public:
	getline_range() = default;

	explicit getline_range(std::istream& in)
		: m_IStream{ &in }
	{
		next();
	}

	explicit getline_range(std::istream& in, const char delimiter)
		: m_IStream{ &in },
		m_Delimiter{ delimiter }
	{
		next();
	}

	[[nodiscard]]
	bool empty() const { return !m_IStream->good(); }

private:
	[[nodiscard]]
	std::string_view read() const noexcept { return m_Line; }

	[[nodiscard]]
	bool equal(ranges::default_sentinel_t) const { return empty(); }

	void next()
	{
		std::getline(*m_IStream, m_Line, m_Delimiter);
	}
};

constexpr auto to_int = [](const std::string_view str)
{
	int temp{};
	std::from_chars(str.data(), str.data() + str.size(), temp);
	return temp;
};
