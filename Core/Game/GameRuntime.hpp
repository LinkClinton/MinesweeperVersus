#pragma once

#include "../../Core/Interface/Noncopyable.hpp"

#include "Enum/GameBlockStatus.hpp"
#include "Enum/GameBlockType.hpp"
#include "Enum/GameCommand.hpp"
#include "Enum/GameStatus.hpp"

#include "GameBoard.hpp"

#include <chrono>
#include <memory>
#include <vector>

#ifdef near
#undef near
#endif

namespace Minesweeper {

	struct GameBlock {
		GameBlockStatus Status = GameBlockStatus::eUnknown;
		GameBlockType Type = GameBlockType::e0;

		GameBlock() = default;

		GameBlock(
			const GameBlockStatus& status,
			const GameBlockType& type);
	};
	
	class GameRuntime : public Noncopyable {
	public:
		explicit GameRuntime(
			const std::shared_ptr<GameBoard>& initGameBoard);

		auto command(const Location& location, const GameCommand& command) 
			-> GameStatus;

		auto near(const Location& location) const -> std::vector<Location>;

		auto nearMines(const Location& location) const -> std::vector<Location>;

		auto nearFlags(const Location& location) const -> std::vector<Location>;

		auto location(const size_t index) const -> Location;
		
		auto index(const Location& location) const -> size_t;

		auto block(const size_t& index) const -> GameBlock;

		auto block(const Location& location) const -> GameBlock;

		auto valid(const Location& location) const noexcept -> bool;

		auto valid(const size_t& index) const noexcept -> bool;

		auto gameStatus() const noexcept -> GameStatus;
		
		auto board() const noexcept -> std::shared_ptr<GameBoard>;
		
		auto blocks() const noexcept -> std::vector<GameBlock>;

		auto nFlags() const noexcept -> size_t;

		auto time() const noexcept -> std::chrono::high_resolution_clock::duration;
	private:
		auto executeCheckStatusCommand() const -> GameStatus;

		auto executeCheckAllCommand(const Location& location) ->GameStatus;

		auto executeCheckCommand(const Location& location) -> GameStatus;
		
		auto executeFlagCommand(const Location& location) -> GameStatus;
	private:
		std::chrono::high_resolution_clock::time_point mStartTime;
		std::chrono::high_resolution_clock::time_point mEndTime;
		
		std::shared_ptr<GameBoard> mInitGameBoard;

		std::vector<GameBlock> mBoardBlocks;

		GameStatus mGameStatus = GameStatus::eCommon;
		
		size_t mFlags = 0;
		size_t mMines = 0;
	};
	
}