#include "ImportMinesGenerator.hpp"

#include <Windows.h>
#include <cassert>

struct Location {
	int X;
	int Y;
};

using GenerateFunction = void(int, int, int&, Location*);

Minesweeper::ImportMinesGenerator::ImportMinesGenerator(const std::string& path)
{
	mLibrary = LoadLibrary(path.c_str());

	assert(mLibrary != nullptr);
}

Minesweeper::ImportMinesGenerator::~ImportMinesGenerator()
{
	if (mLibrary == nullptr) return;
	
	FreeLibrary(static_cast<HMODULE>(mLibrary));

	mLibrary = nullptr;
}

auto Minesweeper::ImportMinesGenerator::get(
	const size_t width, 
	const size_t height,
	const std::optional<size_t>& nMines, 
	const std::any& any) -> std::vector<Location>
{
	const auto function = reinterpret_cast<GenerateFunction*>(GetProcAddress(static_cast<HMODULE>(mLibrary), "generate"));

	const auto pWidth = static_cast<int>(width);
	const auto pHeight = static_cast<int>(height);
	auto pMines = !nMines.has_value() ? -1 : static_cast<int>(nMines.value());

	const auto result = new ::Location[width * height];

	function(pWidth, pHeight, pMines, result);

	auto mines = std::vector<Location>(pMines);

	for (size_t index = 0; index < mines.size(); index++) {
		mines[index].X = static_cast<size_t>(result[index].X);
		mines[index].Y = static_cast<size_t>(result[index].Y);
	}
	
	delete[] result;

	return mines;
}
