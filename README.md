# Minimal Game Engine

## Introduction

This is a incomplete small game engine developed during my college days.

## Dependencies

### Arch Linux
> sudo pacman -S sdl2 sdl2_image sdl2_ttf sdl2_mixer lua glm

### MacOS
> brew install sdl2 sdl2_image sdl2_ttf sdl2_mixer  
> vcpkg install sol2 lua

remember to add the vcpkg and homebrew path to the cmake file, use sdl-config to get the path and
integrate vcpkg install.

uses [sol headers,](https://github.com/ThePhD/sol2) and it is in
the [AUR](https://aur.archlinux.org/packages/sol2) as other option