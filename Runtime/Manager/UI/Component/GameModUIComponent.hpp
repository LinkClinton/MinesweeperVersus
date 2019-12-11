#pragma once

#include "UIComponent.hpp"

namespace Minesweeper {

	class GameModUIComponent : public UIComponent {
	public:
		explicit GameModUIComponent(
			const std::shared_ptr<RuntimeSharing>& sharing);

		~GameModUIComponent() = default;
	private:
		void update();
	private:
		std::pair<std::string, std::string> mCurrent;
	};
	
}