#pragma once

#include "MinesGenerator.hpp"

namespace Minesweeper {

	class RandomMinesGenerator : public MinesGenerator {
	public:
		RandomMinesGenerator() = default;

		~RandomMinesGenerator() = default;

		auto get(
			const size_t width, const size_t height, 
			const std::optional<size_t>& nMines, 
			const std::any& any)
			-> std::vector<Location> override;
	};
	
}