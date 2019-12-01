#include "MinesweeperApp.hpp"

#include "../Extensions/ImGui/imgui_impl_win32.hpp"
#include "../Core/Game/GameContext.hpp"

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

void Minesweeper::MinesweeperApp::initializeCodeRedComponents()
{
	initializeDevice();
	initializeCommand();
	initializeSwapChain();
}

void Minesweeper::MinesweeperApp::initializeManagerComponents()
{
	initializeUIManager();
}

void Minesweeper::MinesweeperApp::initializeGameComponents()
{
	initializeGameContext();
}

void Minesweeper::MinesweeperApp::update(float delta)
{
	mUIManager->update(delta);
}

void Minesweeper::MinesweeperApp::render(float delta)
{
	mCommandQueue->waitIdle();
	mCommandAllocator->reset();

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
}

void Minesweeper::MinesweeperApp::initializeGameContext()
{
	mGameContext = std::make_shared<GameContext>();

	mGameContext->setGameBoard(100, 16, 30);
	mGameContext->startGame();
}

void Minesweeper::MinesweeperApp::processMessage(MinesweeperApp* app, const MSG& message)
{

}
