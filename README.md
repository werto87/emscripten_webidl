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
- git clone https://github.com/werto87/emscripten_webidl.git
- cd emscripten_webidl
- mkdir build
- cd build
- conan install .. --profile:build=<your default profile> --profile:host=emscripten --build missing  -s build_type=Debug
- emcmake cmake .. -DCMAKE_BUILD_TYPE=Debug
- cmake --build .
- cp ../html/some.html .
- python -m http.server
  
Open this link in your browser localhost:8000
Look in the console there should be something like 
Bar(long val)42
42
  
