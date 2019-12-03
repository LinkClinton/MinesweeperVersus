#include "GameFinishUIComponent.hpp"

#include "../../../../Core/Game/GameContext.hpp"

#include "../UIManager.hpp"

Minesweeper::GameFinishUIComponent::GameFinishUIComponent(const std::shared_ptr<RuntimeSharing>& sharing) :
	UIComponent(sharing)
{
	mImGuiView = std::make_shared<CodeRed::ImGuiView>(
		std::bind(&GameFinishUIComponent::update, this));
}

void Minesweeper::GameFinishUIComponent::update()
{
	if (mShow == false) return;

	const auto gameContext = mRuntimeSharing->context();

	static auto imGuiWindowFlags =
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse | 
		ImGuiWindowFlags_AlwaysAutoResize;

	if (ImGui::BeginPopupModal("Message##GameFinish", &mShow, imGuiWindowFlags)) {

		if (ImGui::Button("Start New Game?")) {
			mRuntimeSharing->context()->startGame();
			mShow = false;
		}
		
		updateProperties();

		ImGui::EndPopup();
	}
}
