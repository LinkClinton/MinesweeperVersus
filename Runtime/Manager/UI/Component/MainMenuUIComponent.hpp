#pragma once

#include "UIComponent.hpp"

namespace Minesweeper {

	class MainMenuUIComponent : public UIComponent {
	public:
		explicit MainMenuUIComponent(const std::shared_ptr<RuntimeSharing>& sharing);

		~MainMenuUIComponent() = default;
	private:
		void update();

		auto nameIndex(const std::string & name) const->size_t;

		void initializeWindowsMenus();
	private:
		using Component = std::pair<std::string, std::string>;
		using Components = std::vector<Component>;
		using Menu = std::pair<std::string, Components>;

		std::unordered_map<std::string, size_t> mNameIndices;

		std::vector<Menu> mWindowMenus;
		
	};
	
}