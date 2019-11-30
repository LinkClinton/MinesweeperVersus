#include "GameBoard.hpp"

Minesweeper::GameBoard::GameBoard(
	const std::vector<Location>& mines,
	const size_t boardHeight,
	const size_t boardWidth) :
	mMines(mines), mHeight(boardHeight), mWidth(boardWidth)
{
}

auto Minesweeper::GameBoard::height() const noexcept -> size_t
{
	return mHeight;
}

auto Minesweeper::GameBoard::width() const noexcept -> size_t
{
	return mWidth;
}

auto Minesweeper::GameBoard::mines() const noexcept -> std::vector<Location>
{
	return mMines;
}