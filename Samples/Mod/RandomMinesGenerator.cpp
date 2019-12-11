#include "RandomMinesGenerator.hpp"

#include <random>
#include <ctime>

auto Minesweeper::RandomMinesGenerator::get(
	const size_t width, const size_t height,
	const std::optional<size_t>& nMines, 
	const std::any& any)
	-> std::vector<Location>
{
	std::default_random_engine random(static_cast<unsigned>(time(nullptr)));

	const std::uniform_int_distribution<size_t> xRange(0, width - 1);
	const std::uniform_int_distribution<size_t> yRange(0, height - 1);

	auto mines = std::vector<Location>(nMines.has_value() ? nMines.value() : 0);
	auto has = std::vector<bool>(width * height, false);
	
	for (size_t index = 0; index < mines.size(); index++) {
		auto location = Location(xRange(random), yRange(random));

		while (has[location.Y * width + location.X]) 
			location = Location(xRange(random), yRange(random));

		has[location.Y * width + location.X] = true;

		mines[index] = location;
	}

	return mines;
}
