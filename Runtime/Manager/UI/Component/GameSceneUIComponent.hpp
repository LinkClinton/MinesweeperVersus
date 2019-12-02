#pragma once

#include "UIComponent.hpp"

namespace Minesweeper {

	class GameSceneUIComponent : public UIComponent {
	public:
		explicit GameSceneUIComponent(
			const std::shared_ptr<RuntimeSharing>& sharing);

		~GameSceneUIComponent() = default;
	private:
		void update();
	private:
		std::unordered_map<size_t, size_t> mNextHoverMapped;
	};
	
}