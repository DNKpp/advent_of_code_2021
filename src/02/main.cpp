#include <concepts>
#include <fstream>
#include <ranges>
#include <string>
#include <filesystem>

#include <fmt/format.h>
#include <range/v3/all.hpp>

#include "utility.hpp"

// solves https://adventofcode.com/2021/day/2

void do_part1()
{
	int hPos{ 0 };
	int vPos{ 0 };

	std::string inDir{};
	int inDist{};
	for (std::ifstream in{ std::filesystem::path{ INPUT_DIR } / "input.txt" }; in.good();)
	{
		in >> inDir >> inDist;
		if (inDir == "forward")
		{
			hPos += inDist;
		}
		else
		{
			vPos += inDir == "down" ? inDist : -inDist;
		}
	}

	fmt::print("part 1 answer is: {}\n", hPos * vPos);
}

void do_part2()
{
	int hPos{ 0 };
	int vPos{ 0 };
	int aim{ 0 };

	std::string inDir{};
	int inUnits{};
	for (std::ifstream in{ std::filesystem::path{ INPUT_DIR } / "input.txt" }; in.good();)
	{
		in >> inDir >> inUnits;
		if (inDir == "forward")
		{
			hPos += inUnits;
			vPos += inUnits * aim;
		}
		else
		{
			aim += inDir == "down" ? inUnits : -inUnits;
		}
	}

	fmt::print("part 2 answer is: {}\n", hPos * vPos);
}

int main()
{
	do_part1();

	do_part2();

	return 0;
}
