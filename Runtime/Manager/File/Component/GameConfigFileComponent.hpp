#pragma once

#include "FileComponent.hpp"

namespace Minesweeper {

	class GameConfigFileComponent : public FileComponent {
	public:
		GameConfigFileComponent();
		
		void load(const std::string& fileName) override;

		void save(const std::string& fileName) override;
	public:
		size_t mMines = 100;
		size_t mWidth = 30;
		size_t mHeight = 16;

		bool mSwapMouseButton = false;
		
		std::pair<size_t, size_t> mResolution = { 1920, 1080 };
	};
	
}