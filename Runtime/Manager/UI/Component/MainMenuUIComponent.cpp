#include "MainMenuUIComponent.hpp"

#include "../../Core/Game/GameContext.hpp"

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

	auto openGameConfig = false;
	auto openGameMod = false;
	
	ImGui::BeginMainMenuBar();

	if (ImGui::BeginMenu("Game")) {

		if (ImGui::MenuItem("New")) {
			mRuntimeSharing->context()->startGame();
		}

		if (ImGui::MenuItem("Mod")) {
			mRuntimeSharing->uiManager()->components().at("GameMod")->show();

			openGameMod = true;
		}
		
		if (ImGui::MenuItem("Config")) {
			mRuntimeSharing->uiManager()->components().at("GameConfig")->show();

			openGameConfig = true;
		}
		
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

	if (openGameConfig) ImGui::OpenPopup("GameConfig");
	if (openGameMod) ImGui::OpenPopup("GameMod");
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
