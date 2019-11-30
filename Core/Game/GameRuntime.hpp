#pragma once

#include "../../Core/Interface/Noncopyable.hpp"

#include "Enum/GameBlockStatus.hpp"
#include "Enum/GameBlockType.hpp"

#include "GameBoard.hpp"

#include <memory>
#include <vector>

namespace Minesweeper {

	class GameRuntime : public Noncopyable {
	public:
		explicit GameRuntime(
			const std::shared_ptr<GameBoard>& initGameBoard);

	private:
		std::shared_ptr<GameBoard> mInitGameBoard;

		std::vector<GameBlockStatus> mBoardBlockStatus;
		std::vector<GameBlockType> mBoardBlockType;
	};
	
}