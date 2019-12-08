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

		ImGui::PopStyleColor();

		updateProperties();

		ImGui::EndPopup();
	}
}
