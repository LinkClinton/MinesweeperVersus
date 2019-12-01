#include "RuntimeSharing.hpp"

#include "MinesweeperApp.hpp"

Minesweeper::RuntimeSharing::RuntimeSharing(MinesweeperApp* minesweeperApp)
	: mMinesweeperApp(minesweeperApp)
{
	
}

auto Minesweeper::RuntimeSharing::uiManager() const noexcept -> std::shared_ptr<UIManager>
{
	return mMinesweeperApp->mUIManager;
}

auto Minesweeper::RuntimeSharing::allocator() const noexcept -> std::shared_ptr<CodeRed::GpuCommandAllocator>
{
	return mMinesweeperApp->mCommandAllocator;
}

auto Minesweeper::RuntimeSharing::device() const noexcept -> std::shared_ptr<CodeRed::GpuLogicalDevice>
{
	return mMinesweeperApp->mDevice;
}

auto Minesweeper::RuntimeSharing::queue() const noexcept -> std::shared_ptr<CodeRed::GpuCommandQueue>
{
	return mMinesweeperApp->mCommandQueue;
}

auto Minesweeper::RuntimeSharing::context() const noexcept -> std::shared_ptr<GameContext>
{
	return mMinesweeperApp->mGameContext;
}
