# AdventureGame

Cpp Adventure game

## Setup

install sfml

```shell
sudo pacman -S sfml
```

create output folder if it doesn't exist

```shell
mkdir -p ./output 
```

build project

```shell
g++ draw_map.cpp -std=c++20 ./src/*.cpp -o ./output/main -lsfml-graphics -lsfml-window -lsfml-system
```

run project

```shell
./output/main
```

build & run project

```shell
g++ draw_map.cpp -std=c++20 ./src/*.cpp -o ./output/main -lsfml-graphics -lsfml-window -lsfml-system && ./output/main
```
