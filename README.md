# Placeholder Game Engine

The main engine. Vroom!

## Dependencies

Windows should come with libraries and a visual studio project already set up (soon tm). For Linux, you'll need access to the following libraries:

- SDL2 (libsdl2-dev)
- SDL2 Image (libsdl2-image-dev)
- SDL2 TTF (libsdl2-ttf-dev)
- SDL2 Mixer (libsdl2-mixer-dev)
- Boost (libboost-all-dev) (TODO: Build Windows binaries)
- [Sol2] (https://github.com/ThePhD/sol2)
- Lua 5.3 (liblua5.3-dev) (TODO: Build Windows binaries) 
- Lua 5.3 Runtime (lua5.3)
- Sol2 (Already in depend)

The headers *should* also exist inside the `libs/include/` folder.

## Development and testing

### In general:

The only folder which contains assets for the game itself should be `system/` folder (which will exist with a .placeholder file in it). This *should* be empty,
as most assets should be placed in here (manifests included) and all that data should be pulled off some external system (google drive/One Drive/???).

There is a .gitignore in this repo which should keep it out of any general `git add` statements (i.e. `git add *`)

### *nix machines:

Just run cmake and it **should** work (tm).

### Windows machines (Visual Studio):

Once the Visual Studio project is set up, you just need Visual Studio 2017 Community (or something similar, though that will be untested) and you can compile straight
from Visual Studio (build etc.)

JH: Note, I don't really use windows to dev, most of this will not be rigorously tested until we're ready to push to prod.

### Windows machines (non-Visual Studio):

JH: Unfortunately you'll be shit-outta-luck ATM because I don't have a development environment like this.
