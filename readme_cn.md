# MinesweeperVersus 

[`English`](./readme.md) [`中文`](./readme_cn.md) 

`MinesweeperVersus`是一个轻量的支持模组(生成器)以及`AI`接口的扫雷游戏。玩家可以自己编写生成器以及实现一些AI算法来完成这个游戏。

## How to play

### Release

第一个版本已经放出。点击[这里](https://github.com/LinkClinton/MinesweeperVersus/releases)去获取。

### Input

- 鼠标左键，可以单击未知的块翻开它。
- 鼠标右键，可以单击来给一个未知的块标记成雷，如果单击一个已知的块，那么会检测附近的块，将能够推测出来的块翻开。

### Block Type

- `1...8` 表示的是附近块存在的雷的数量。
- `X` 表示的是一块雷。如果你看到了它，你可能就游戏结束了。
- `F` 表示棋子，即这一个未知块将会被当作一个雷。

### Config

- `Width` 游戏盘的宽度。
- `Height` 游戏盘的高度。
- `mines` 一局游戏的雷数。
- `SwapMouseButton` 如果他是真，那么就互换鼠标左右键。
- `Resolution` 游戏的分辨率。
- `Generator` 使用的生成器，如果生成器不存在那么就使用默认的随机生成器。

### Mod

模组页面将会显示`./Resources/Mods/`文件夹里面的模组。目前只支持生成器。

## How to build

### Game

- 克隆当前项目。
- 克隆 [CodeRed](https://github.com/LinkClinton/Code-Red) 以及它的子模块到 `./References/`.
- 使用 Vcpkg 安装第三方库.
- 使用 Visutal Studio 2019 构建Runtime项目(只支持X64)。

### Mod

如果你只是要构建模组，你不需要任何第三方库。

模组有一个文件夹构成，里面有两个文件。第一个是一个`json`文件，名字为`description`用来存放模组的名字，作者和描述信息。第二个是一个动态链接库，用来存放编译后的生成器的代码。动态链接库里面必须存在一个叫做`generate`的函数，定义如下。

```C
struct Location{
    int X, Y;
};

void generate(int width, int height, int& nMines, Location* result);
```

**注意：函数的调用方式必须为cdecl.**

### ThirdParties

ImGui 没有必要使用 Vcpkg 去安装，因为它被包含在子模块里面。

- [Json](https://github.com/nlohmann/json) : JSON for Modern C++.
- [ImGui](https://github.com/ocornut/imgui) : Dear ImGui: Bloat-free Immediate Mode Graphical User interface for C++ with minimal dependencies.