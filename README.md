# Mr. Tako's Trials (20 Second Jam)

A small minigame collection made for 2022's **[20 Second Game Jam](https://itch.io/jam/20-second-game-jam)**,
where the goal of the jam was to create a game that lasts 20 seconds. I bent the rules a little bit and
instead made a collection of minigames where each game would last 20 seconds. A download for the game can
be found over on **[itch.io](https://jrob774.itch.io/mr-takos-trials)** where there is also a browser version
available.

## Development

The game was developed in C using **[SDL2](https://libsdl.org/)**, with the web port being developed using
**[emscripten](https://github.com/emscripten-core/emscripten)**.

## Building

After cloning the repository you will need to run `git submodule update --init --recursive` to setup submodules.

The game can then be built by running the `build.bat` script with a build profile as the first argument. The
following build profiles are available:

- `win32` which builds the Windows version of the game.
- `web` which builds the web version of the game.
- `tools` which builds auxiliary tools used for development.
- `assets` which will build the final assets for the game.

The Windows build also accepts an extra argument `release` which can be used to build the  optimized release
executable of the game.

The asset build job invokes a script which converts the raw Photoshop texture asset files into compiled texture
atlases that the actual game uses. It is necessary to run this at least once or else the game will not be able
to find most of the assets. This job does require **[Python](https://www.python.org/)** in order to be executed.

## License

The project's code is available under the **[MIT License](https://github.com/JROB774/mr-takos-trials/blob/master/LICENSE)**.
