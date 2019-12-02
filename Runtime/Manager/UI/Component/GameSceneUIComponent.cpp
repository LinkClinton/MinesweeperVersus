#include "GameSceneUIComponent.hpp"

#include "../../../../Core/Game/GameContext.hpp"

#include "../UIManager.hpp"

Minesweeper::GameSceneUIComponent::GameSceneUIComponent(const std::shared_ptr<RuntimeSharing>& sharing) :
	UIComponent(sharing)
{
	mImGuiView = std::make_shared<CodeRed::ImGuiView>(
		std::bind(&GameSceneUIComponent::update, this));
}

void Minesweeper::GameSceneUIComponent::update()
{
	if (mShow == false) return;

	const auto width = mRuntimeSharing->context()->board()->width();
	const auto height = mRuntimeSharing->context()->board()->height();
	const auto gameContext = mRuntimeSharing->context();

	static auto imGuiWindowFlags =
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoTitleBar;

	ImGui::Begin("##GameScene", nullptr, imGuiWindowFlags);

	const auto mainMenuHeight = mRuntimeSharing->uiManager()->components().at("MainMenu")->size().y;

	const auto size = ImGui::GetWindowSize();
	const auto offsetSize = ImVec2(
		size.x - (ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x),
		size.y - (ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y)
	);

	const auto extent = std::min(
		mRuntimeSharing->uiManager()->width() * 0.8f / width,
		mRuntimeSharing->uiManager()->height() * 0.8f / height);

	ImGui::SetWindowSize(ImVec2(
		extent * width + offsetSize.x,
		(extent + 1) * height + offsetSize.y
	));

	ImGui::SetWindowPos(ImVec2(
		(mRuntimeSharing->uiManager()->width() - ImGui::GetWindowSize().x) * 0.5f,
		mainMenuHeight + (mRuntimeSharing->uiManager()->height() - ImGui::GetWindowSize().y - mainMenuHeight) * 0.5f
	));

	static const char* typeMapped[] = {
		" ##", "1##", "2##", "3##", "4##", "5##", "6##", "7##", "8##", "X##"
	};

	const auto hoverMap = mNextHoverMapped;
	const auto blockSize = ImVec2(
		extent - ImGui::GetStyle().FramePadding.y,
		extent - ImGui::GetStyle().FramePadding.y);

	mNextHoverMapped.clear();
	
	for (size_t y = 0; y < height; y++) {
		for (size_t x = 0; x < width; x++) {

			const auto location = Location(x, y);
			const auto block = gameContext->runtime()->block(location);
			const auto index = gameContext->runtime()->index(location);
			const auto color = hoverMap.find(index) != hoverMap.end() || block.Status == GameBlockStatus::eKnown ?
				ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered) :
				ImGui::GetStyleColorVec4(ImGuiCol_Button);

			ImGui::PushStyleColor(ImGuiCol_Button, color);

			if (block.Status == GameBlockStatus::eFlag) {
				ImGui::Button(("F##" + std::to_string(index)).c_str() , blockSize);
				
				//when we click the right button
				if (ImGui::IsItemClicked(1)) {
					gameContext->runtime()->command(location, GameCommand::eFlag);
				}
			}

			if (block.Status == GameBlockStatus::eUnknown) {
				if (ImGui::Button((" ##" + std::to_string(index)).c_str(), blockSize)) {
					gameContext->runtime()->command(location, GameCommand::eCheck);
				}

				//when we click the right button
				if (ImGui::IsItemClicked(1)) {
					gameContext->runtime()->command(location, GameCommand::eFlag);
				}
			}

			if (block.Status == GameBlockStatus::eKnown) {
				ImGui::Button((typeMapped[enumConvert(block.Type)] + std::to_string(index)).c_str(), blockSize);

				if (ImGui::IsMouseDown(1) && ImGui::IsItemHovered())
				{
					gameContext->runtime()->command(location, GameCommand::eCheckAll);

					const auto nearBlocks = gameContext->runtime()->near(location);

					for (const auto& nearBlock : nearBlocks) {
						if (gameContext->runtime()->block(nearBlock).Status == GameBlockStatus::eFlag)
							continue;

						mNextHoverMapped.insert({
							gameContext->runtime()->index(nearBlock),
							gameContext->runtime()->index(nearBlock) });
					}
				}
			}
			
			ImGui::PopStyleColor();

			if (x != width - 1) 
				ImGui::SameLine(0, ImGui::GetStyle().FramePadding.y);
		}
	}

	updateProperties();

	ImGui::End();
}
