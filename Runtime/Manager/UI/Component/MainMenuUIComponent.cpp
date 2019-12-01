#include "MainMenuUIComponent.hpp"

#include "../UIManager.hpp"

Minesweeper::MainMenuUIComponent::MainMenuUIComponent(const std::shared_ptr<RuntimeSharing>& sharing) :
	UIComponent(sharing)
{
	mImGuiView = std::make_shared<CodeRed::ImGuiView>(
		std::bind(&MainMenuUIComponent::update, this));

	initializeWindowsMenus();
}

void Minesweeper::MainMenuUIComponent::update()
{
	if (mShow == false) return;

	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("Game")) {

		ImGui::MenuItem("New");
		
		ImGui::EndMenu();
	}

	for (const auto& menu : mWindowMenus) {
		if (ImGui::BeginMenu(menu.first.c_str())) {

			for (const auto& item : menu.second) {
				if (ImGui::MenuItem(item.first.c_str())) {
					mRuntimeSharing->uiManager()->components().at(item.second)->show();
				}
			}

			ImGui::EndMenu();
		}
	}

	updateProperties();

	ImGui::EndMainMenuBar();
}

auto Minesweeper::MainMenuUIComponent::nameIndex(const std::string& name) const -> size_t
{
	return mNameIndices.at(name);
}

void Minesweeper::MainMenuUIComponent::initializeWindowsMenus()
{
	for (size_t index = 0; index < mWindowMenus.size(); index++)
		mNameIndices.insert({ mWindowMenus[index].first, index });
}
