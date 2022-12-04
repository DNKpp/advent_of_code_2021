#include <bitset>
#include <concepts>
#include <fstream>
#include <ranges>
#include <string>
#include <filesystem>
#include <tuple>
#include <sstream>
#include <optional>
#include <list>

#include <fmt/format.h>
#include <range/v3/all.hpp>
#include <experimental/mdarray>

#include "utility.hpp"

// solves https://adventofcode.com/2021/day/4

using board = std::experimental::mdarray<std::optional<int>, std::experimental::extents<int, 5, 5>>;

inline std::optional<std::tuple<int, int>> find_cell(const board& b, const int value)
{
	for (const auto [i, j] : ranges::views::cartesian_product(
			ranges::views::iota(0, b.extent(0)),
			ranges::views::iota(0, b.extent(1))
		))
	{
		if (b(i, j) == value)
		{
			return std::tuple{ i, j };
		}
	}

	return std::nullopt;
}

inline bool row_finished(const board& b, const int index)
{
	return std::ranges::all_of(
		std::views::iota(0, b.extent(0)),
		[&](const int i) { return !b(index, i); }
	);
}

inline bool column_finished(const board& b, const int index)
{
	return std::ranges::all_of(
		std::views::iota(0, b.extent(1)),
		[&](const int i) { return !b(i, index); }
	);
}

inline int calc_board_score(const board& b)
{
	return ranges::accumulate(
		b.container(),
		0,
		std::plus{},
		[](const std::optional<int>& v) { return v.value_or(0); }
	);
}

inline std::tuple<int, board> find_first_winning(std::vector<board> boards, const std::vector<int>& rngValues)
{
	for (auto&& [value, curBoard] : ranges::views::cartesian_product(rngValues, boards))
	{
		if (auto at = find_cell(curBoard, value))
		{
			const auto [r, c] = *at;
			curBoard(r, c) = std::nullopt;
			if (row_finished(curBoard, r) || column_finished(curBoard, c))
			{
				return { value, curBoard };
			}
		}
	}

	throw -1;
}

inline std::tuple<int, board> find_last_winning(std::vector<board> boards, const std::vector<int>& rngValues)
{
	for (std::vector<board> temp{}; const int value : rngValues)
	{
		std::ranges::remove_copy_if(
			boards,
			std::back_inserter(temp),
			[&](board& curBoard)
			{
				if (auto at = find_cell(curBoard, value))
				{
					const auto [r, c] = *at;
					curBoard(r, c) = std::nullopt;
					if (row_finished(curBoard, r) || column_finished(curBoard, c))
					{
						return true;
					}
				}
				return false;
			}
		);

		if (temp.empty())
		{
			return { value, boards.front() };
		}

		boards = std::move(temp);
		temp.clear();
	}

	throw -1;
}

inline std::tuple<std::vector<int>, std::vector<board>> read_input()
{
	std::ifstream in{ std::filesystem::path{ INPUT_DIR } / "input.txt" };

	std::istringstream rngStream{
		[&]
		{
			std::string line{};
			std::getline(in, line);
			return line;
		}()
	};

	return {
		getline_range{ rngStream, ',' }
		| ranges::views::transform(to_int)
		| ranges::to_vector,
		[&]
		{
			std::vector<board> boards{};
			for (std::istream_iterator<int> iter{ in }, end{}; iter != end;)
			{
				iter = std::ranges::copy_n(
					iter,
					25,
					boards.emplace_back().container().begin()
				).in;
			}

			return boards;
		}()
	};
}

void do_part1()
{
	auto [rngValues, boards] = read_input();

	const auto& [value, board] = find_first_winning(std::move(boards), rngValues);
	const int boardScore = calc_board_score(board);

	fmt::print("part 1 answer is: {}\n", boardScore * value);
}

void do_part2()
{
	auto [rngValues, boards] = read_input();

	const auto& [value, board] = find_last_winning(std::move(boards), rngValues);
	const int boardScore = calc_board_score(board);

	fmt::print("part 2 answer is: {}\n", boardScore * value);
}

int main()
{
	do_part1();

	do_part2();

	return 0;
}
