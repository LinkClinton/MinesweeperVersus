#include "FileManager.hpp"

#include "Component/GameConfigFileComponent.hpp"

Minesweeper::FileManager::FileManager(const std::shared_ptr<RuntimeSharing>& sharing) :
	Manager(sharing)
{
}

void Minesweeper::FileManager::initialize()
{
	addComponent("GameConfig", std::make_shared<GameConfigFileComponent>());
}

void Minesweeper::FileManager::finalize()
{
	for (const auto& component : mFileComponents) 
		component.second->finalize();
	
	mFileComponents.clear();
}

void Minesweeper::FileManager::addComponent(const std::string& name, const std::shared_ptr<FileComponent>& component)
{
	component->initialize();

	mFileComponents.insert({ name, component });
}

void Minesweeper::FileManager::removeComponent(const std::string& name)
{
	mFileComponents[name]->finalize();
	mFileComponents.erase(name);
}

auto Minesweeper::FileManager::components() const noexcept -> const std::map<std::string, std::shared_ptr<FileComponent>>& 
{
	return mFileComponents;
}
