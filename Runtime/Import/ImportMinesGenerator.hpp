#pragma once

#include "../../Core/Algorithm/MinesGenerator/MinesGenerator.hpp"

#include <string>

namespace Minesweeper {

	class ImportMinesGenerator : public MinesGenerator {
	public:
		explicit ImportMinesGenerator(const std::string& path);

		~ImportMinesGenerator();

		auto get(
			const size_t width, 
			const size_t height, 
			const std::optional<size_t>& nMines, 
			const std::any& any) -> std::vector<Location> override;
	private:
		void* mLibrary = nullptr;
	};
	
}