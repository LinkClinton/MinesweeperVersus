#include "GameRuntime.hpp"

#include <queue>

Minesweeper::GameBlock::GameBlock(const GameBlockStatus& status, const GameBlockType& type) :
	Status(status), Type(type)
{
}

Minesweeper::GameRuntime::GameRuntime(const std::shared_ptr<GameBoard>& initGameBoard) :
	mInitGameBoard(initGameBoard), mBoardBlocks(mInitGameBoard->width() * mInitGameBoard->height()),
	mMines(initGameBoard->mines().size())
{
	const auto mines = mInitGameBoard->mines();

	//update the mine of block
	for (const auto& mine : mines) {
		mBoardBlocks[index(mine)].Status = GameBlockStatus::eUnknown;
		mBoardBlocks[index(mine)].Type = GameBlockType::eMine;
	}

	//update the type of block, such as e0, e1, e2...
	for (size_t index = 0; index < mBoardBlocks.size(); index++) {
		//we do not need update the mine
		if (mBoardBlocks[index].Type == GameBlockType::eMine) continue;
		
		const auto location = GameRuntime::location(index);

		mBoardBlocks[index].Type = toGameBlockType(nearMines(location).size());
		mBoardBlocks[index].Status = GameBlockStatus::eUnknown;
	}

	mStartTime = std::chrono::high_resolution_clock::now();
}

auto Minesweeper::GameRuntime::command(const Location& location, const GameCommand& command) -> GameStatus
{
	if (mGameStatus != GameStatus::eCommon) return mGameStatus;
	
	switch (command) {
	case GameCommand::eCheckAll: mGameStatus = executeCheckAllCommand(location); break;
	case GameCommand::eCheck: mGameStatus = executeCheckCommand(location); break;
	case GameCommand::eFlag: mGameStatus = executeFlagCommand(location); break;
	default: throw "No Command";
	}

	//when the game is finished, we need update the end time
	if (mGameStatus != GameStatus::eCommon)
		mEndTime = std::chrono::high_resolution_clock::now();
	
	return mGameStatus;
}

auto Minesweeper::GameRuntime::near(const Location& location) const -> std::vector<Location>
{
	const static int XOffset[8] = { -1, +0, +1, -1, +1, -1, +0, +1 };
	const static int YOffset[8] = { -1, -1, -1, +0, +0, +1, +1, +1 };

	auto locations = std::vector<Location>();
	
	for (size_t index = 0; index < 8; index++) {
		const auto nearLocation = Location(
			location.X + XOffset[index],
			location.Y + YOffset[index]
		);

		if (!valid(nearLocation)) continue;

		locations.push_back(nearLocation);
	}

	return locations;
}

auto Minesweeper::GameRuntime::nearMines(const Location& location) const -> std::vector<Location>
{
	const auto nearBlocks = near(location);

	auto mines = std::vector<Location>();

	for (const auto& block : nearBlocks) {
		if (mBoardBlocks[index(block)].Type == GameBlockType::eMine)
			mines.push_back(block);
	}

	return mines;
}

auto Minesweeper::GameRuntime::nearFlags(const Location& location) const -> std::vector<Location>
{
	const auto nearBlocks = near(location);

	auto flags = std::vector<Location>();
	
	for (const auto& block : nearBlocks) {
		if (mBoardBlocks[index(block)].Status == GameBlockStatus::eFlag)
			flags.push_back(block);
	}

	return flags;
}

auto Minesweeper::GameRuntime::location(const size_t index) const -> Location
{
	return Location(
		index % mInitGameBoard->width(),
		index / mInitGameBoard->width()
	);
}

auto Minesweeper::GameRuntime::index(const Location& location) const -> size_t
{
	return location.Y * mInitGameBoard->width() + location.X;
}

auto Minesweeper::GameRuntime::block(const size_t& index) const -> GameBlock
{
	return mBoardBlocks[index];
}

auto Minesweeper::GameRuntime::block(const Location& location) const -> GameBlock
{
	return mBoardBlocks[index(location)];
}

auto Minesweeper::GameRuntime::valid(const Location& location) const noexcept -> bool
{
	return
		location.X < mInitGameBoard->width() &&
		location.Y < mInitGameBoard->height();
}

auto Minesweeper::GameRuntime::valid(const size_t& index) const noexcept -> bool
{
	return index < mBoardBlocks.size();
}

auto Minesweeper::GameRuntime::gameStatus() const noexcept -> GameStatus
{
	return mGameStatus;
}

auto Minesweeper::GameRuntime::board() const noexcept -> std::shared_ptr<GameBoard>
{
	return mInitGameBoard;
}

auto Minesweeper::GameRuntime::blocks() const noexcept -> std::vector<GameBlock>
{
	return mBoardBlocks;
}

auto Minesweeper::GameRuntime::nFlags() const noexcept -> size_t
{
	return mFlags;
}

auto Minesweeper::GameRuntime::time() const noexcept -> std::chrono::high_resolution_clock::duration
{
	if (mEndTime != std::chrono::high_resolution_clock::time_point())
		return mEndTime - mStartTime;
	else
		return std::chrono::high_resolution_clock::now() - mStartTime;
}

auto Minesweeper::GameRuntime::executeCheckStatusCommand() const -> GameStatus
{
	for (const auto& block : mBoardBlocks) {
		if (block.Type != GameBlockType::eMine && block.Status != GameBlockStatus::eKnown)
			return GameStatus::eCommon;
	}

	return GameStatus::eWin;

	/*if (mFlags != mMines) return GameStatus::eCommon;

	for (const auto& mine : mInitGameBoard->mines()) {
		if (mBoardBlocks[index(mine)].Status != GameBlockStatus::eFlag) return GameStatus::eCommon;
	}

	return GameStatus::eWin;*/
}

auto Minesweeper::GameRuntime::executeCheckAllCommand(const Location& start) -> GameStatus
{
	auto isVisited = std::vector<bool>(mBoardBlocks.size(), false);
	auto queue = std::queue<Location>();

	isVisited[index(start)] = true;

	queue.push(start);

	while (!queue.empty()) {
		const auto location = queue.front(); queue.pop();
		const auto index = GameRuntime::index(location);

		//if the block is not known, we can not run check all command
		//so we only ignore this command
		if (mBoardBlocks[index].Status != GameBlockStatus::eKnown) continue;

		const auto nearMines = GameRuntime::nearMines(location);
		const auto nearFlags = GameRuntime::nearFlags(location);

		//if the number of near flags and the number of near mines are not same
		//we can not execute this command
		if (nearMines.size() != nearFlags.size()) continue;

		const auto nearBlocks = near(location);

		for (const auto &block : nearBlocks) {
			const auto blockIndex = GameRuntime::index(block);

			//we do not need to update the block that has flag
			//or we had visited
			if (mBoardBlocks[blockIndex].Status == GameBlockStatus::eFlag ||
				isVisited[blockIndex]) continue;

			mBoardBlocks[blockIndex].Status = GameBlockStatus::eKnown;

			//if the flags are not right, we will get mines
			//so we lost the game
			if (mBoardBlocks[blockIndex].Type == GameBlockType::eMine) return GameStatus::eLost;

			isVisited[blockIndex] = true;

			queue.push(block);
		}
	}

	return executeCheckStatusCommand();
}

auto Minesweeper::GameRuntime::executeCheckCommand(const Location& start) -> GameStatus
{
	auto isVisited = std::vector<bool>(mBoardBlocks.size(), false);
	auto queue = std::queue<Location>();

	isVisited[index(start)] = true;

	queue.push(start);

	while (!queue.empty()) {
		const auto location = queue.front(); queue.pop();
		const auto index = GameRuntime::index(location);

		mBoardBlocks[index].Status = GameBlockStatus::eKnown;

		//if we get a mine, we lost the game
		if (mBoardBlocks[index].Type == GameBlockType::eMine) return GameStatus::eLost;

		//if we get a zero block, it means the near blocks are not mine
		//so we can check these blocks
		if (mBoardBlocks[index].Type == GameBlockType::e0) {
			const auto nearBlocks = near(location);

			for (const auto& block : nearBlocks) {
				const auto blockIndex = GameRuntime::index(block);

				if (isVisited[blockIndex]) continue;

				isVisited[blockIndex] = true;

				queue.push(block);
			}
		}
	}

	return executeCheckStatusCommand();
}

auto Minesweeper::GameRuntime::executeFlagCommand(const Location& location) -> GameStatus
{
	const auto index = GameRuntime::index(location);
	
	switch (mBoardBlocks[index].Status) {
	case GameBlockStatus::eUnknown: mBoardBlocks[index].Status = GameBlockStatus::eFlag; mFlags++; break;
	case GameBlockStatus::eFlag: mBoardBlocks[index].Status = GameBlockStatus::eUnknown; mFlags--; break;
	case GameBlockStatus::eKnown: return GameStatus::eCommon;
	default:
		throw "No Game Status";
	}

	return executeCheckStatusCommand();
}
