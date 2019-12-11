#include "GameModUIComponent.hpp"

#include "../../File/Component/GameConfigFileComponent.hpp"
#include "../../File/Component/GameModFileComponent.hpp"

#include "../../../../Runtime/MinesweeperApp.hpp"
#include "../../../../Core/Game/GameContext.hpp"

#include "../../File/FileManager.hpp"
#include "../UIManager.hpp"

Minesweeper::GameModUIComponent::GameModUIComponent(const std::shared_ptr<RuntimeSharing>& sharing) :
	UIComponent(sharing)
{
	mImGuiView = std::make_shared<CodeRed::ImGuiView>(
		std::bind(&GameModUIComponent::update, this));
}

void Minesweeper::GameModUIComponent::update()
{
	if (mShow == false) return;

	const auto gameContext = mRuntimeSharing->context();
	const auto gameConfig = std::static_pointer_cast<GameConfigFileComponent>(
		mRuntimeSharing->fileManager()->components().at("GameConfig"));
	const auto gameMod = std::static_pointer_cast<GameModFileComponent>(
		mRuntimeSharing->fileManager()->components().at("GameMod"));

	static auto imGuiWindowFlags =
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse;

	if (ImGui::BeginPopupModal("GameMod", &mShow, imGuiWindowFlags)) {

		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.f, 0.f, 0.f, 0.1f));

		ImGui::SetWindowSize(ImVec2(
			mRuntimeSharing->uiManager()->width() * 0.5f,
			mRuntimeSharing->uiManager()->height() * 0.5f
		));

		auto modsName = std::vector<const char*>(gameMod->mods().size());
		auto current = 0;
		
		for (size_t index = 0; index < modsName.size(); index++) {
			if (mCurrent == std::make_pair(gameMod->mods()[index].mName, gameMod->mods()[index].mAuthor))
				current = static_cast<int>(index);
			
			modsName[index] = gameMod->mods()[index].mName.c_str();
		}
		
		ImGui::ListBox("##ModList", &current, modsName.data(), 
			static_cast<int>(modsName.size()), 
			static_cast<int>(mRuntimeSharing->uiManager()->height() / 54));

		ImGui::SameLine();

		if (!gameMod->mods().empty()) {
			ImGui::TextWrapped("Name   : %s\nAuthor : %s\n\n\n%s\n",
				gameMod->mods()[current].mName.c_str(),
				gameMod->mods()[current].mAuthor.c_str(),
				gameMod->mods()[current].mDescription.c_str());

			mCurrent = { gameMod->mods()[current].mName, gameMod->mods()[current].mAuthor };
		}else {
			mCurrent = { "", "" };
		}
		
		ImGui::PopStyleColor();

		updateProperties();

		ImGui::EndPopup();
		
	}
}
