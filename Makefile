main:
	clang++ -std=c++17 -O3 -I./ThirdParty/glm/ -I./ThirdParty/rapidjson-1.1.0/include/ -I./SDL2/ -I./SDL2_mixer/ -I./SDL2_ttf/ -I./SDL_image/ -I./LuaBridge/ -I./box2d/ -I./box2dSRC/ -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -llua5.4 *.cpp box2dSRC/collision/*.cpp box2dSRC/common/*.cpp box2dSRC/dynamics/*.cpp box2dSRC/rope/*.cpp -o game_engine_linux

