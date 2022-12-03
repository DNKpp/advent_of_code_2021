#include <bitset>
#include <concepts>
#include <fstream>
#include <ranges>
#include <string>
#include <filesystem>
#include <tuple>

#include <fmt/format.h>
#include <range/v3/all.hpp>

#include "utility.hpp"

// solves https://adventofcode.com/2021/day/3

static constexpr int bitWidth{ 12 };
using bits = std::bitset<bitWidth>;
using bit_counter = std::array<int, bitWidth>;

std::tuple<int, bit_counter> determine_common_bits(std::ranges::input_range auto&& r)
	requires std::convertible_to<std::ranges::range_value_t<decltype(r)>, bits>
{
	int count{ 0 };
	bit_counter bitCounter{};
	std::ranges::for_each(
		r,
		[&](const auto& v)
		{
			++count;
			for (auto&& [id, counter] : ranges::views::enumerate(bitCounter))
			{
				counter += static_cast<int>(v[id]);
			}
		}
	);

	return { count, bitCounter };
}

void do_part1()
{
	std::ifstream in{ std::filesystem::path{ INPUT_DIR } / "input.txt" };
	const auto& [count, bitCounter] = determine_common_bits(std::ranges::views::istream<bits>(in));

	const auto gamma = ranges::accumulate(
		bitCounter | ranges::views::reverse,
		bits{ 0ul },
		[majorityThreshold = count / 2](const bits curGamma, const int bitValue)
		{
			return curGamma << 1 | static_cast<bits>(majorityThreshold <= bitValue);
		}
	);

	const auto epsilon = bits{ gamma }.flip();

	fmt::print("part 1 answer is: {}\n", gamma.to_ullong() * epsilon.to_ullong());
}

template <bool VMajority>
bits search(const std::vector<bits>& values)
{
	std::ranges::subrange filtered(values.begin(), values.end());

	for (auto bitIndex : ranges::views::iota(0, bitWidth)
						| ranges::views::reverse
						| ranges::views::take_while([&](auto) { return filtered.size() != 1; })
	)
	{
		const auto middle = std::ranges::lower_bound(filtered, true, std::less{}, [&](const bits& v) { return v[bitIndex]; });
		const auto onesCount = std::ranges::distance(middle, filtered.end());
		const auto zeroesCount = std::ranges::distance(filtered.begin(), middle);
		const bool isLowerBorder = (zeroesCount <= onesCount) == VMajority;
		filtered = isLowerBorder
						? std::ranges::subrange{ middle, filtered.end() }
						: std::ranges::subrange{ filtered.begin(), middle };
	}

	return filtered.front();
}

void do_part2()
{
	const std::vector inputValues = cache_input<bits>()
									| ranges::actions::sort(std::less{}, &bits::to_ulong);

	const auto ogr = search<true>(inputValues);
	const auto co2 = search<false>(inputValues);

	fmt::print("part 2 answer is: {}\n", ogr.to_ullong() * co2.to_ullong());
}

int main()
{
	do_part1();

	do_part2();

	return 0;
}
