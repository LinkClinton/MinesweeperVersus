#pragma once

namespace Minesweeper {

	struct Location {
		size_t X = 0;
		size_t Y = 0;

		Location() = default;

		Location(
			const size_t x,
			const size_t y) :
			X(x), Y(y) {}
	};

	
}