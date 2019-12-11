#pragma once

#include "../../Interface/Noncopyable.hpp"
#include "../../Shared/Location.hpp"

#include <optional>
#include <vector>
#include <any>

namespace Minesweeper {
	
	class MinesGenerator : public Noncopyable {
	public:
		MinesGenerator() = default;

		~MinesGenerator() = default;

		virtual auto get(
			const size_t width, const size_t height, 
			const std::optional<size_t>& nMines = std::nullopt, 
			const std::any& any = nullptr)
			-> std::vector<Location> = 0;
	};
	
}