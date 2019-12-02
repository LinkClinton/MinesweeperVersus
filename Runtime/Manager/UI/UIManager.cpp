#include "UIManager.hpp"

#include "Component/GameFinishUIComponent.hpp"
#include "Component/GameConfigUIComponent.hpp"
#include "Component/GameStatusUIComponent.hpp"
#include "Component/GameSceneUIComponent.hpp"
#include "Component/MainMenuUIComponent.hpp"

Minesweeper::UIManager::UIManager(
	const std::shared_ptr<RuntimeSharing>& sharing,
	const std::shared_ptr<CodeRed::GpuLogicalDevice>& device,
	const std::shared_ptr<CodeRed::GpuRenderPass>& renderPass,
	const std::shared_ptr<CodeRed::GpuCommandAllocator>& allocator,
	const std::shared_ptr<CodeRed::GpuCommandQueue>& queue,
	const size_t width, const size_t height) : Manager(sharing),
	mDevice(device), mRenderPass(renderPass),
	mCommandAllocator(allocator), mCommandQueue(queue),
	mWidth(width), mHeight(height)
{
	mCommandList = mDevice->createGraphicsCommandList(mCommandAllocator);

	ImGui::GetIO().Fonts->AddFontFromFileTTF("./Resources/Fonts/consola.ttf", 20);
	
	mImGuiWindows = std::make_shared<CodeRed::ImGuiWindows>(
		mDevice,
		mRenderPass,
		mCommandAllocator,
		mCommandQueue, 2);

	addComponent("MainMenu", std::make_shared<MainMenuUIComponent>(mRuntimeSharing));
	addComponent("GameScene", std::make_shared<GameSceneUIComponent>(mRuntimeSharing));
	addComponent("GameStatus", std::make_shared<GameStatusUIComponent>(mRuntimeSharing));
	addComponent("GameConfig", std::make_shared<GameConfigUIComponent>(mRuntimeSharing));
	addComponent("GameFinish", std::make_shared<GameFinishUIComponent>(mRuntimeSharing));
}

void Minesweeper::UIManager::update(float delta)
{
	mImGuiWindows->update();
}

auto Minesweeper::UIManager::render(const std::shared_ptr<CodeRed::GpuFrameBuffer>& frameBuffer, float delta)
-> std::shared_ptr<CodeRed::GpuGraphicsCommandList>
{
	mCommandList->beginRecording();
	mCommandList->beginRenderPass(mRenderPass, frameBuffer);

	mImGuiWindows->draw(mCommandList);

	mCommandList->endRenderPass();
	mCommandList->endRecording();

	return mCommandList;
}

void Minesweeper::UIManager::addComponent(const std::string& name, const std::shared_ptr<UIComponent>& component)
{
	mUIComponents.insert({ name, component });

	mImGuiWindows->add(name, component->view());
}

void Minesweeper::UIManager::removeComponent(const std::string& name)
{
	mUIComponents.erase(name);

	mImGuiWindows->remove(name);
}

void Minesweeper::UIManager::resize(const size_t width, const size_t height)
{
	mWidth = width;
	mHeight = height;
}

auto Minesweeper::UIManager::width() const noexcept -> size_t
{
	return mWidth;
}

auto Minesweeper::UIManager::height() const noexcept -> size_t
{
	return mHeight;
}

auto Minesweeper::UIManager::components() const noexcept -> const std::map<std::string, std::shared_ptr<UIComponent>>&
{
	return mUIComponents;
}