#include "GameConfigUIComponent.hpp"

#include "../../../../Core/Game/GameContext.hpp"

#include "../UIManager.hpp"

Minesweeper::GameConfigUIComponent::GameConfigUIComponent(const std::shared_ptr<RuntimeSharing>& sharing) :
	UIComponent(sharing)
{
	mImGuiView = std::make_shared<CodeRed::ImGuiView>(
		std::bind(&GameConfigUIComponent::update, this));
}

auto Minesweeper::GameConfigUIComponent::isSwapMouseButton() const noexcept -> bool
{
	return mSwapMouseButton;
}

void Minesweeper::GameConfigUIComponent::update()
{
	if (mShow == false) return;

	const auto gameContext = mRuntimeSharing->context();
	
	static auto imGuiWindowFlags =
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse;

	if (ImGui::BeginPopupModal("GameConfig", &mShow, imGuiWindowFlags)) {

		ImGui::SetWindowSize(ImVec2(
			mRuntimeSharing->uiManager()->width() * 0.5f,
			mRuntimeSharing->uiManager()->height() * 0.5f
		));

		auto nMines = static_cast<int>(gameContext->board()->mines().size());
		auto nHeight = static_cast<int>(gameContext->board()->height());
		auto nWidth = static_cast<int>(gameContext->board()->width());
		auto isChanged = false;
		
		isChanged ^= ImGui::InputInt("Mines", &nMines);
		isChanged ^= ImGui::InputInt("Width", &nWidth);
		isChanged ^= ImGui::InputInt("Height", &nHeight);

		ImGui::Checkbox("SwapMouseButton", &mSwapMouseButton);

		nHeight = std::clamp(nHeight, 9, 16);
		nWidth = std::clamp(nWidth, 9, 30);
		nMines = std::clamp(nMines, 1, nWidth * nHeight - 1);

		if (isChanged) {
			gameContext->setGameBoard(nMines, nHeight, nWidth);
			
			gameContext->startGame();
		}
		
		updateProperties();

		ImGui::EndPopup();
	}
}
