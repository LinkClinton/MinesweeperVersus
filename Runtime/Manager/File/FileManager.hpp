#pragma once

#include "Component/FileComponent.hpp"
#include "../Manager.hpp"

#include <map>

namespace Minesweeper {

	class FileManager : public Manager {
	public:
		explicit FileManager(
			const std::shared_ptr<RuntimeSharing>& sharing);

		void initialize() override;

		void finalize() override;

		void addComponent(
			const std::string& name,
			const std::shared_ptr<FileComponent>& component);

		void removeComponent(const std::string& name);

		auto components() const noexcept -> const std::map<std::string, std::shared_ptr<FileComponent>>&;
	private:
		std::map<std::string, std::shared_ptr<FileComponent>> mFileComponents;

	};
	
}