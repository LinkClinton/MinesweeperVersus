#include "MinesweeperApp.hpp"

#include "../Extensions/ImGui/imgui_impl_win32.hpp"

#include "../Core/Game/GameContext.hpp"

#include "Import/ImportMinesGenerator.hpp"

#include "Manager/File/Component/GameConfigFileComponent.hpp"
#include "Manager/File/Component/GameModFileComponent.hpp"
#include "Manager/File/FileManager.hpp"
#include "Manager/UI/UIManager.hpp"
#include "RuntimeSharing.hpp"

#include <chrono>

#define __DIRECTX12__MODE__
#ifndef __DIRECTX12__MODE__
#define __VULKAN__MODE__
#endif

using Time = std::chrono::high_resolution_clock;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT DefaultWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
	default:
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

Minesweeper::MinesweeperApp::MinesweeperApp(const std::string& name, size_t width, size_t height)
	: mName(name), mWidth(width), mHeight(height), mHwnd(nullptr), mExisted(false)
{
	mRuntimeSharing = std::make_shared<RuntimeSharing>(this);
	
	const auto hInstance = GetModuleHandle(nullptr);
	const auto class_name = this->name();

	WNDCLASS appInfo;

	appInfo.style = CS_DBLCLKS;
	appInfo.lpfnWndProc = DefaultWindowProc;
	appInfo.cbClsExtra = 0;
	appInfo.cbWndExtra = 0;
	appInfo.hInstance = hInstance;
	appInfo.hIcon = static_cast<HICON>(LoadImage(nullptr, "", IMAGE_ICON, 0, 0, LR_LOADFROMFILE));
	appInfo.hCursor = LoadCursor(nullptr, IDC_ARROW);
	appInfo.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	appInfo.lpszMenuName = nullptr;
	appInfo.lpszClassName = &class_name[0];

	RegisterClass(&appInfo);

	RECT rect;

	rect.top = 0;
	rect.left = 0;
	rect.right = static_cast<UINT>(this->width());
	rect.bottom = static_cast<UINT>(this->height());

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	mHwnd = CreateWindow(&class_name[0], &class_name[0], WS_OVERLAPPEDWINDOW ^
		WS_SIZEBOX ^ WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, hInstance, nullptr);

	mExisted = true;

	ImGui::CreateContext();
	ImGui_ImplWin32_Init(mHwnd);

	initializeCodeRedComponents();
	initializeManagerComponents();
	initializeGameComponents();
}

Minesweeper::MinesweeperApp::~MinesweeperApp()
{
	mFileManager->finalize();
	
	mUIManager->finalize();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	mCommandQueue->waitIdle();
}

void Minesweeper::MinesweeperApp::show() const
{
	ShowWindow(mHwnd, SW_SHOW);
}

void Minesweeper::MinesweeperApp::hide() const
{
	ShowWindow(mHwnd, SW_HIDE);
}

void Minesweeper::MinesweeperApp::runLoop()
{
	auto currentTime = Time::now();

	while (mExisted == true) {
		MSG message;

		message.hwnd = mHwnd;

		while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&message);
			DispatchMessage(&message);

			processMessage(this, message);

			if (message.message == WM_QUIT) mExisted = false;
		}

		if (mExisted == false) break;

		auto duration = std::chrono::duration_cast<
			std::chrono::duration<float>>(Time::now() - currentTime);

		currentTime = Time::now();

		ImGui_ImplWin32_NewFrame();
		
		update(duration.count());
		render(duration.count());
	}
}

void Minesweeper::MinesweeperApp::resize(const size_t width, const size_t height)
{
	mWidth = width;
	mHeight = height;

	RECT rect;

	rect.top = 0;
	rect.left = 0;
	rect.right = static_cast<UINT>(mWidth);
	rect.bottom = static_cast<UINT>(mHeight);

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
	
	SetWindowPos(mHwnd, nullptr, 0, 0, 
		rect.right - rect.left,
		rect.bottom - rect.top,
		SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOREPOSITION);

	mUIManager->resize(mWidth, mHeight);
}

void Minesweeper::MinesweeperApp::initializeCodeRedComponents()
{
	initializeDevice();
	initializeCommand();
	initializeSwapChain();
}

void Minesweeper::MinesweeperApp::initializeManagerComponents()
{
	initializeFileManager();
	initializeUIManager();
}

void Minesweeper::MinesweeperApp::initializeGameComponents()
{
	initializeGameContext();
}

void Minesweeper::MinesweeperApp::update(float delta)
{
	mUIManager->update(delta);

	const auto gameConfigFileComponent = 
		std::static_pointer_cast<GameConfigFileComponent>(mFileManager->components().at("GameConfig"));

	if (gameConfigFileComponent->mResolution.first != mWidth ||
		gameConfigFileComponent->mResolution.second != mHeight)
		resize(
			gameConfigFileComponent->mResolution.first,
			gameConfigFileComponent->mResolution.second);
}

void Minesweeper::MinesweeperApp::render(float delta)
{
	mCommandQueue->waitIdle();
	mCommandAllocator->reset();

	if (mSwapChain->width() != mWidth || mSwapChain->height() != mHeight) {
		mFrameBuffers.clear();
		mSwapChain->resize(mWidth, mHeight);

		for (size_t index = 0; index < mSwapChain->bufferCount(); index++) {
			mFrameBuffers.push_back(
				mDevice->createFrameBuffer(
					mSwapChain->buffer(index)
				)
			);
		}
		
		mCurrentFrameIndex = 0;
	}

	auto commandLists = std::vector<std::shared_ptr<CodeRed::GpuGraphicsCommandList>>();

	commandLists.push_back(mUIManager->render(mFrameBuffers[mCurrentFrameIndex], delta));
	
	mCommandQueue->execute(commandLists);

	mSwapChain->present();

	mCurrentFrameIndex = (mCurrentFrameIndex + 1) % mSwapChain->bufferCount();
}

void Minesweeper::MinesweeperApp::initializeDevice()
{
#ifdef __DIRECTX12__MODE__
	auto systemInfo = std::make_shared<CodeRed::DirectX12SystemInfo>();
	auto adapters = systemInfo->selectDisplayAdapter();
	
	mDevice = std::make_shared<CodeRed::DirectX12LogicalDevice>(adapters[0]);
#else
	auto systemInfo = std::make_shared<CodeRed::VulkanSystemInfo>();
	auto adapters = systemInfo->selectDisplayAdapter();

	mDevice = std::make_shared<CodeRed::VulkanLogicalDevice>(adapters[0]);
#endif
}

void Minesweeper::MinesweeperApp::initializeCommand()
{
	mCommandAllocator = mDevice->createCommandAllocator();
	mCommandQueue = mDevice->createCommandQueue();
}

void Minesweeper::MinesweeperApp::initializeSwapChain()
{
	mSwapChain = mDevice->createSwapChain(
		mCommandQueue,
		{ mWidth, mHeight, mHwnd },
		CodeRed::PixelFormat::BlueGreenRedAlpha8BitUnknown,
		2
	);

	for (size_t index = 0; index < mSwapChain->bufferCount(); index++) {
		mFrameBuffers.push_back(
			mDevice->createFrameBuffer(
				mSwapChain->buffer(index)
			)
		);
	}

	mRenderPass = mDevice->createRenderPass(
		CodeRed::Attachment::RenderTarget(mSwapChain->format())
	);

	mRenderPass->setClear(CodeRed::ClearValue());
}

void Minesweeper::MinesweeperApp::initializeFileManager()
{
	mFileManager = std::make_shared<FileManager>(mRuntimeSharing);

	mFileManager->initialize();
}

void Minesweeper::MinesweeperApp::initializeUIManager()
{
	ImGui::StyleColorsLight();

	mUIManager = std::make_shared<UIManager>(
		mRuntimeSharing,
		mDevice,
		mRenderPass,
		mCommandAllocator,
		mCommandQueue,
		width(), height());

	mUIManager->initialize();
}

void Minesweeper::MinesweeperApp::initializeGameContext()
{
	auto gameConfig = std::static_pointer_cast<GameConfigFileComponent>(
		mFileManager->components().at("GameConfig"));
	auto gameMod = std::static_pointer_cast<GameModFileComponent>(
		mFileManager->components().at("GameMod"));
	
	mGameContext = std::make_shared<GameContext>();

	for (size_t index = 0; index < gameMod->mods().size(); index++) {
		const auto mod = gameMod->mods()[index];
		
		if (gameConfig->mGenerator.first == mod.mName &&
			gameConfig->mGenerator.second == mod.mAuthor) {
			mGameContext->setMinesGenerator(std::make_shared<ImportMinesGenerator>(mod.mPath));
		}
	}
	
	mGameContext->setGameBoard(gameConfig->mMines, gameConfig->mHeight, gameConfig->mWidth);
	mGameContext->startGame();
}

void Minesweeper::MinesweeperApp::processMessage(MinesweeperApp* app, const MSG& message)
{

}
