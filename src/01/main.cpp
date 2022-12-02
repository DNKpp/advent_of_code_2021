#include <concepts>
#include <fstream>
#include <ranges>
#include <string>
#include <filesystem>

#include <fmt/format.h>
#include <range/v3/all.hpp>

#include "utility.hpp"

// solves https://adventofcode.com/2021/day/1

void do_part1()
{
	const std::vector input = cache_input<int>();

	const auto increasedCount = std::ranges::count(
		ranges::views::zip_with(std::less{}, input, input | ranges::views::drop(1)),
		true
	);

	fmt::print("part 1 answer is: {}\n", increasedCount);
}

void do_part2()
{
	const std::vector input = cache_input<int>();

	auto window = input
				| ranges::views::sliding(3)
				| ranges::views::transform([](const std::ranges::input_range auto& r) { return ranges::accumulate(r, 0); });

	const auto increasedCount = std::ranges::count(
		ranges::views::zip_with(std::less{}, window, window | ranges::views::drop(1)),
		true
	);

	fmt::print("part 2 answer is: {}\n", increasedCount);
}

int main()
{
	do_part1();

	do_part2();

	return 0;
}
