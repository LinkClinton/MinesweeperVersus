#pragma once

#include <optional>
#include <vector>
#include <any>

namespace Minesweeper {

	class Noncopyable {
	public:
		Noncopyable() = default;
		virtual ~Noncopyable() = default;

		Noncopyable(const Noncopyable&) = delete;
		Noncopyable(Noncopyable&&) noexcept = default;

		Noncopyable& operator=(const Noncopyable&) = delete;
		Noncopyable& operator=(Noncopyable&&) = default;
	};

	struct Location {
		size_t X = 0;
		size_t Y = 0;

		Location() = default;

		Location(
			const size_t x,
			const size_t y) :
			X(x), Y(y) {}
	};
	
	class MinesGenerator : public Noncopyable {
	public:
		MinesGenerator() = default;

		~MinesGenerator() = default;

		virtual auto get(
			const size_t width, const size_t height,
			const std::optional<size_t>& nMines = std::nullopt,
			const std::any& any = nullptr)
			->std::vector<Location> = 0;
	};


}