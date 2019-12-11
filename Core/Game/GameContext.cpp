#include "GameContext.hpp"

#include "../Algorithm/MinesGenerator/RandomMinesGenerator.hpp"

Minesweeper::GameContext::GameContext() :
	mMines(100), mBoardHeight(16), mBoardWidth(30)
{
	mMinesGenerator = std::make_shared<RandomMinesGenerator>();
}

void Minesweeper::GameContext::setMinesGenerator(const std::shared_ptr<MinesGenerator>& generator)
{
	mMinesGenerator = generator;
}

void Minesweeper::GameContext::setGameBoard(
	const std::optional<size_t> nMines, 
	const size_t boardHeight,
	const size_t boardWidth)
{
	mMines = nMines;
	mBoardHeight = boardHeight;
	mBoardWidth = boardWidth;
}

void Minesweeper::GameContext::startGame()
{
	const auto mines = mMinesGenerator->get(mBoardWidth, mBoardHeight, mMines);
	
	mGameBoard = std::make_shared<GameBoard>(mines, mBoardHeight, mBoardWidth);
	mGameRuntime = std::make_shared<GameRuntime>(mGameBoard);
}

auto Minesweeper::GameContext::minesGenerator() const noexcept -> std::shared_ptr<MinesGenerator>
{
	return mMinesGenerator;
}

auto Minesweeper::GameContext::runtime() const noexcept -> std::shared_ptr<GameRuntime>
{
	return mGameRuntime;
}

auto Minesweeper::GameContext::board() const noexcept -> std::shared_ptr<GameBoard>
{
	return mGameBoard;
}