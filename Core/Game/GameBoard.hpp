#pragma once

#include "../Algorithm/MinesGenerator/MinesGenerator.hpp"
#include "../Interface/Noncopyable.hpp"

#include <vector>

namespace Minesweeper {

	class GameBoard : public Noncopyable {
	public:
		explicit GameBoard(
			const std::vector<Location>& mines,
			const size_t boardHeight,
			const size_t boardWidth);

		~GameBoard() = default;

		auto height() const noexcept->size_t;
		
		auto width() const noexcept -> size_t;

		auto mines() const noexcept -> std::vector<Location>;
	private:
		std::vector<Location> mMines;
		
		size_t mHeight;
		size_t mWidth;
	};
	
}