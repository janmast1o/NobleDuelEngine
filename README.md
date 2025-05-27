# NobleDuelEngine

### (Still in development, current version on 'refactored' branch)

A simple 2D physics engine written in C++ (C++17), using SDL2 as the only external library. Developed on WSL Ubuntu.

### Showcase:

If you do not wish to clone the repo and run the code on your machine, please consider visiting the following page: [janmast1o.github.io/noble_duel_engine_showcase_dir/](https://janmast1o.github.io/noble_duel_engine_showcase_dir/), which provides a video showcasing some of the engine capabilities.

### External library installation:

In the ubuntu terminal please run the following commands:

`sudo apt update`


`sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev`


These commands should install the necessary libraries and make code compilation possible.

### How to compile:

1. Clone a repo to a local directory:

`git clone <this website's address>`

2. Make sure that the refactored branch is pulled, then create a directory within the repo directory and name it 'compiled':

`cd NobleDuelEngine`


`git fetch`


`git checkout refactored`


`mkdir compiled`

3. Run a make within the now cloned repository:

`make`

4. Run the program:

`compiled/main`

### Controls:

Use A and B to move left and right, space to jump and hold down Left Shift/V to move quickly/very slowly.


