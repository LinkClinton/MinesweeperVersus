#pragma once

#include "../References/Code-Red/CodeRed/Interface/GpuLogicalDevice.hpp"

#include "../Core/Interface/Noncopyable.hpp"

#include <memory>

namespace Minesweeper {

	class MinesweeperApp;
	class GameContext;
	class UIManager;
	
	class RuntimeSharing : public Noncopyable {
	public:
		explicit RuntimeSharing(MinesweeperApp* minesweeperApp);

		auto uiManager() const noexcept -> std::shared_ptr<UIManager>;

		auto app() const noexcept -> MinesweeperApp*;
		
		auto allocator() const noexcept -> std::shared_ptr<CodeRed::GpuCommandAllocator>;

		auto device() const noexcept -> std::shared_ptr<CodeRed::GpuLogicalDevice>;

		auto queue() const noexcept -> std::shared_ptr<CodeRed::GpuCommandQueue>;

		auto context() const noexcept -> std::shared_ptr<GameContext>;
	private:
		MinesweeperApp* mMinesweeperApp;
	};

}