#pragma once

#include "UIComponent.hpp"

namespace Minesweeper {

	class GameConfigUIComponent : public UIComponent {
	public:
		explicit GameConfigUIComponent(
			const std::shared_ptr<RuntimeSharing>& sharing);

		~GameConfigUIComponent() = default;
	private:
		void update();
	};
	
}