#include "GameConfigFileComponent.hpp"

#include "../../Extensions/Json/Json.hpp"

#include <fstream>
#include <iomanip>

Minesweeper::GameConfigFileComponent::GameConfigFileComponent() :
	FileComponent("gameConfig.json", FileComponentMask::eAll)
{
}

void Minesweeper::GameConfigFileComponent::load(const std::string& fileName)
{
	auto file = std::ifstream(fileName);
	auto config = Json();

	if (!file.is_open()) return;

	file >> config;
	
	mHeight = config["height"];
	mWidth = config["width"];
	mMines = config["mines"];

	mSwapMouseButton = config["swapMouseButton"];
	mResolution.first = config["resolution"]["width"];
	mResolution.second = config["resolution"]["height"];

	file.close();
}

void Minesweeper::GameConfigFileComponent::save(const std::string& fileName)
{
	auto file = std::ofstream(fileName);
	
	assert(file.is_open());

	const Json config = {
		{ "height", mHeight },
		{ "width", mWidth },
		{ "mines", mMines },
		{ "swapMouseButton", mSwapMouseButton },
		{ "resolution",
			{
				{ "width", mResolution.first },
				{ "height", mResolution.second }
			}
		}
	};

	file << std::setw(4) << config;
	file.close();
}
