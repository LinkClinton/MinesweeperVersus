#include "RandomMinesGenerator.hpp"

#include <memory>

struct Location {
	int X = 0;
	int Y = 0;
};

void generate(int width, int height, int& nMines, Location* result) {
	auto generator = std::make_shared<Minesweeper::RandomMinesGenerator>();

	auto mines = generator->get(
		static_cast<size_t>(width),
		static_cast<size_t>(height),
		nMines < 0 ? std::nullopt : std::optional<size_t>(nMines),
		nullptr
	);

	nMines = static_cast<int>(mines.size());

	for (size_t index = 0; index < mines.size(); index++) {
		result[index].X = static_cast<int>(mines[index].X);
		result[index].Y = static_cast<int>(mines[index].Y);
	}
}