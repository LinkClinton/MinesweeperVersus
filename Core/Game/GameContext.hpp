#pragma once

#include "../Interface/Noncopyable.hpp"

#include "GameRuntime.hpp"

namespace Minesweeper {

	class GameContext : public Noncopyable {
	public:
		GameContext();

		~GameContext() = default;

		void setMinesGenerator(
			const std::shared_ptr<MinesGenerator>& generator);
		
		void setGameBoard(
			const std::optional<size_t> nMines,
			const size_t boardHeight,
			const size_t boardWidth);

		void startGame();

		auto minesGenerator() const noexcept -> std::shared_ptr<MinesGenerator>;

		auto runtime() const noexcept -> std::shared_ptr<GameRuntime>;

		auto board() const noexcept -> std::shared_ptr<GameBoard>;
	private:
		std::shared_ptr<MinesGenerator> mMinesGenerator;
		std::shared_ptr<GameRuntime> mGameRuntime;
		std::shared_ptr<GameBoard> mGameBoard;
		
		std::optional<size_t> mMines;
		size_t mBoardHeight;
		size_t mBoardWidth;
	};
	
}