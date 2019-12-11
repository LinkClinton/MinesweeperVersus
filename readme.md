# MinesweeperVersus 

[`中文`](./readme_cn.md) [`English`](./readme.md)

MinesweeperVersus is a light minesweeper game support mods(generator) and AI. Player can write yourself mines generator to generate the mines and try some AI algorithm to solve the game.

## How to play

### Release

First version is released. Click [here](https://github.com/LinkClinton/MinesweeperVersus/releases) to get.

### Input

- Left Mouse Button can click a unknown block to set it known(like other minesweeper game).
- Right Mouse Button can flag a unknown block or check a known block to set blocks around it to known.

### Block Type

- `1...8` means there are some mines around this block. And the number is the number of mines.
- `X` means it is a mine. If you see it, you may lost game.
- `F` means it is a flag. We use flag to flag a block and treat it as mine.

### Config

- `Width` the width of game board.
- `Height` the height of game board.
- `mines` the number of mines.
- `SwapMouseButton` if true, we will swap left mouse button and right mouse button.
- `Resolution` the resolution of game.
- `Generator` which generator we will use. If the generator is not exist we will use default generator(random mines).

### Mod

The mod page will show mods in `./Resources/Mods/`. Now, we only support mines generator mods.

## How to build

### Game

- Clone this repository.
- Clone [CodeRed](https://github.com/LinkClinton/Code-Red) and sub-modules to `./References/`.
- Using Vcpkg to install the thirdparties.
- Build `Runtime` with `X64` using Visual Studio 2019.

### Mod

If you want only build mods, you do not need any thirdparties.

The mods is a filefolder with two files. First, a `.json` file called `description` to show the name, author, description of mod. Second, a `.dll` file called `generator` is a dynamic link library. The dynamic link library must contain a function called `generate` and with these arguments.

```C
struct Location{
    int X, Y;
};

void generate(int width, int height, int& nMines, Location* result);
```

**Notice : ensure the calling convention is cdecl.**

### ThirdParties

You do not need install ImGui with vcpkg.

- [Json](https://github.com/nlohmann/json) : JSON for Modern C++.
- [ImGui](https://github.com/ocornut/imgui) : Dear ImGui: Bloat-free Immediate Mode Graphical User interface for C++ with minimal dependencies.

