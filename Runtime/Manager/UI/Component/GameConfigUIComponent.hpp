#pragma once

#include "UIComponent.hpp"

namespace Minesweeper {

	class GameConfigUIComponent : public UIComponent {
	public:
		explicit GameConfigUIComponent(
			const std::shared_ptr<RuntimeSharing>& sharing);

		~GameConfigUIComponent() = default;

		auto isSwapMouseButton() const noexcept -> bool;
	private:
		void update();
	private:
		bool mSwapMouseButton = false;
	};
	
}