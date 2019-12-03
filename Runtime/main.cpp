#include "MinesweeperApp.hpp"

int main() {
	Minesweeper::MinesweeperApp app = Minesweeper::MinesweeperApp("Minesweeper", 1280, 720);

	app.show();
	app.runLoop();
}