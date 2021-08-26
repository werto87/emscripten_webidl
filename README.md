# emscripten_webidl
## Preconditions
- [conan](https://conan.io/)
- [cmake](https://cmake.org/)
- [emscripten](https://emscripten.org/docs/getting_started/downloads.html)
## How to run the Project

run:
- conan profile new emscripten
- [copy the content in .conan/profile/emscripten](https://gitlab.com/werto87/conan-the-example/-/blob/master/emscripten) 
- conan remote add gitlab https://gitlab.com/api/v4/projects/27217743/packages/conan
- git clone 
- cd emscripten_webidl
- rm -rf build/
- mkdir build
- cd build
- conan install .. --profile:build=<your default profile> --profile:host=emscripten --build missing  -s build_type=Debug
- emcmake cmake .. -DCMAKE_BUILD_TYPE=Debug
- cmake --build .

