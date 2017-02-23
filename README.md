# gba-games

![battleship](./doc/battleship-1.png)
![battleship](./doc/battleship-2.png)
![battleship](./doc/battleship-3.png)
![megatroid](./doc/megatroid-1.png)
![megatroid](./doc/megatroid-2.png)
![megatroid](./doc/megatroid-3.png)
![pong](./doc/pong-1.png)
![pong](./doc/pong-2.png)
![pong](./doc/pong-3.png)
![tetris](./doc/tetris-1.png)
![tetris](./doc/tetris-2.png)
![tetris](./doc/tetris-3.png)

This repository includes gameboy advance games written using devkitpro.

To facilitate building the following docker image is used: [werner/devkitpro](https://hub.docker.com/r/werner/devkitpro/)

To build use the following:

    make

The generated gameboy advance roms can then be played using an emulator (VisualBoyAdvance):

    VisualBoyAdvance battleship/battleship.gba
    VisualBoyAdvance megatroid/megatroid.gba
    VisualBoyAdvance pong/pong.gba
    VisualBoyAdvance tetris/tetris.gba
