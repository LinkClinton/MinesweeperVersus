#pragma once

#include "UIComponent.hpp"

namespace Minesweeper {

	class GameFinishUIComponent : public UIComponent {
	public:
		explicit GameFinishUIComponent(
			const std::shared_ptr<RuntimeSharing>& sharing);

		~GameFinishUIComponent() = default;
	private:
		void update();
	};

}