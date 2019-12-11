#include "GameConfigUIComponent.hpp"

#include "../../File/Component/GameConfigFileComponent.hpp"
#include "../../File/Component/GameModFileComponent.hpp"

#include "../../../../Runtime/MinesweeperApp.hpp"
#include "../../../../Core/Game/GameContext.hpp"

#include "../../File/FileManager.hpp"
#include "../UIManager.hpp"

Minesweeper::GameConfigUIComponent::GameConfigUIComponent(const std::shared_ptr<RuntimeSharing>& sharing) :
	UIComponent(sharing)
{
	mImGuiView = std::make_shared<CodeRed::ImGuiView>(
		std::bind(&GameConfigUIComponent::update, this));
}

void Minesweeper::GameConfigUIComponent::update()
{
	if (mShow == false) return;

	const auto gameContext = mRuntimeSharing->context();
	const auto gameMod = std::static_pointer_cast<GameModFileComponent>(
		mRuntimeSharing->fileManager()->components().at("GameMod"));
	const auto gameConfig = std::static_pointer_cast<GameConfigFileComponent>(
			mRuntimeSharing->fileManager()->components().at("GameConfig"));

	static auto imGuiWindowFlags =
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse;

	if (ImGui::BeginPopupModal("GameConfig", &mShow, imGuiWindowFlags)) {

		ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.f, 0.f, 0.f, 0.1f));
		
		ImGui::SetWindowSize(ImVec2(
			mRuntimeSharing->uiManager()->width() * 0.5f,
			mRuntimeSharing->uiManager()->height() * 0.5f
		));

		auto nMines = static_cast<int>(gameConfig->mMines);
		auto nHeight = static_cast<int>(gameConfig->mHeight);
		auto nWidth = static_cast<int>(gameConfig->mWidth);
		auto isChanged = false;
		
		isChanged ^= ImGui::InputInt("Mines", &nMines);
		isChanged ^= ImGui::InputInt("Width", &nWidth);
		isChanged ^= ImGui::InputInt("Height", &nHeight);

		ImGui::Checkbox("SwapMouseButton", &gameConfig->mSwapMouseButton);

		nHeight = std::clamp(nHeight, 9, 16);
		nWidth = std::clamp(nWidth, 9, 30);
		nMines = std::clamp(nMines, 1, nWidth * nHeight - 1);

		ImGui::Separator();

		const static std::pair<size_t, size_t> Resolution[] = {
			{ 1920, 1080 },
			{ 1280, 720 }
		};

		const auto currentResolution =
			std::to_string(mRuntimeSharing->app()->width()) + "x" +
			std::to_string(mRuntimeSharing->app()->height());
		
		if (ImGui::BeginCombo("Resolution", currentResolution.c_str())) {
			for (size_t index = 0; index < 2; index++) {
				const auto selected = (
					Resolution[index].first == mRuntimeSharing->app()->width() &&
					Resolution[index].second == mRuntimeSharing->app()->height());

				const auto resolution =
					std::to_string(Resolution[index].first) + "x" +
					std::to_string(Resolution[index].second);
				
				if (ImGui::Selectable(resolution.c_str(), selected)) {
					gameConfig->mResolution = Resolution[index];
				}

				if (selected) ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		if (ImGui::BeginCombo("Generator", gameConfig->mGenerator.first.c_str())) {
			for (size_t index = 0; index < gameMod->mods().size(); index++) {
				const auto mod = gameMod->mods()[index];
				
				const auto selected = (
					gameConfig->mGenerator.first == mod.mName &&
					gameConfig->mGenerator.second == mod.mAuthor);

				if (ImGui::Selectable(mod.mName.c_str(), selected)) {
					gameConfig->mGenerator.first = mod.mName;
					gameConfig->mGenerator.second = mod.mAuthor;
				}

				if (selected) ImGui::SetItemDefaultFocus();
			}
			
			ImGui::EndCombo();
		}
		
		if (isChanged) {
			gameConfig->mHeight = nHeight;
			gameConfig->mMines = nMines;
			gameConfig->mWidth = nWidth;

			gameContext->setGameBoard(nMines, nHeight, nWidth);
			
			gameContext->startGame();
		}

		ImGui::PopStyleColor();
		
		updateProperties();

		ImGui::EndPopup();
	}
}
