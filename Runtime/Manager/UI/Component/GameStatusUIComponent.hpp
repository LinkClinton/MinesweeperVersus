#pragma once

#include "UIComponent.hpp"

namespace Minesweeper {

	class GameStatusUIComponent : public UIComponent {
	public:
		explicit GameStatusUIComponent(
			const std::shared_ptr<RuntimeSharing>& sharing);

		~GameStatusUIComponent() = default;
	private:
		void update();
	};
	
}