# Flappy Eye

This repository is just a Flappy Bird clone written in C language and built
with [CMake](https://cmake.org/) for building and [vcpkg](https://vcpkg.io/)
for dependency management.

## Assets

Some of the assets were provided with a Creative Commons Zero (CC0) license,
while others were free to use, but provided no rights to redistribute. The CC0
assets are provided in the assets folder with the proper credits for the authors.
The non-CC0 need to be downloaded in order for you to build and run the game.
Here are the missing assets that you must download on your own:

### [Free Effect and Bullet 16x16](https://bdragon1727.itch.io/free-effect-and-bullet-16x16)

Download it to any folder. For the asset to work with the spritesheet definition
in the code, you need to extract the fire ball sprite from the single file. The
recommended way is to use [imagemagick](https://imagemagick.org/script/download.php#windows).

You can use the following command to extract the fire ball sprite:
```bash
magick "Fire Effect and Bullet 16x16.png" -crop 48x32+160+160 +repage fire.png
```

Then, move this file to the `assets/sprites` folder of the project.
