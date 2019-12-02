#include "GameStatusUIComponent.hpp"

#include "../../../../Core/Game/GameContext.hpp"

#include "../UIManager.hpp"

auto timeToString(const std::chrono::high_resolution_clock::duration& time) -> std::string
{
	const long long minutes = std::chrono::duration_cast<std::chrono::minutes>(time).count();
	const long long seconds = std::chrono::duration_cast<std::chrono::seconds>(time).count();

	std::string result = "";

	result = result + (minutes < 10 ? "0" : "");
	result = result + std::to_string(minutes) + ":";
	result = result + ((seconds - minutes * 60) < 10 ? "0" : "");
	result = result + std::to_string(seconds - minutes * 60);

	return result;
}

Minesweeper::GameStatusUIComponent::GameStatusUIComponent(const std::shared_ptr<RuntimeSharing>& sharing) :
	UIComponent(sharing)
{
	mImGuiView = std::make_shared<CodeRed::ImGuiView>(
		std::bind(&GameStatusUIComponent::update, this));
}

void Minesweeper::GameStatusUIComponent::update()
{
	if (mShow == false) return;

	const auto gameContext = mRuntimeSharing->context();

	static auto imGuiWindowFlags =
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoTitleBar | 
		ImGuiWindowFlags_AlwaysAutoResize;

	const auto mainMenuHeight = mRuntimeSharing->uiManager()->components().at("MainMenu")->size().y;
	const auto gameScenePositionY = mRuntimeSharing->uiManager()->components().at("GameScene")->position().y;
	
	ImGui::Begin("##GameStatus", nullptr, imGuiWindowFlags);
	
	ImGui::SetWindowPos(ImVec2(
		(mRuntimeSharing->uiManager()->width() - ImGui::GetWindowSize().x) * 0.5f,
		mainMenuHeight + (gameScenePositionY - mainMenuHeight - ImGui::GetWindowSize().y) * 0.5f
	));

	static const char* gameStatusMapped[] = {
		"Common", "Lost", "Win"
	};
	
	const auto flags = static_cast<unsigned>(gameContext->runtime()->nFlags());
	const auto space = ImGui::GetWindowSize().y * 0.2f;
	const auto time = gameContext->runtime()->time();

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(space, space));
	ImGui::Spacing();
	ImGui::Text("Flags : %u", flags); ImGui::SameLine();
	ImGui::Spacing(); ImGui::SameLine();
	ImGui::Spacing(); ImGui::SameLine();
	ImGui::Text("GameStatus : %s", gameStatusMapped[static_cast<unsigned>(gameContext->runtime()->gameStatus())]); ImGui::SameLine();
	ImGui::Spacing(); ImGui::SameLine();
	ImGui::Spacing(); ImGui::SameLine();
	ImGui::Spacing(); ImGui::SameLine();
	ImGui::Text("Time :  %s", timeToString(time).c_str());
	ImGui::PopStyleVar();
	
	updateProperties();

	ImGui::End();
}
