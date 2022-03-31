# emscripten_webidl
Exampel of running c++ in java script. It is a reduced and renamed version of [port to emscripten of c++ bullet](https://github.com/kripken/ammo.js/tree/main).
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
- conan install .. --profile:build=default --profile:host=emscripten --build missing  -s build_type=Debug
- emcmake cmake .. -DCMAKE_BUILD_TYPE=Debug
- cmake --build .
- cp ../html/some.html .
- python -m http.server

Open this link in your browser localhost:8000
Look in the console there should be something like:  
 ```
Pass java script array to c++ do calc and print result in java script some.html:15:21
0 some.html:34:25
0 some.html:35:25
42 some.html:34:25
1337 some.html:35:25
84 some.html:34:25
2674 some.html:35:25
126 some.html:34:25
4011 some.html:35:25
Multiply by 10 in c++ some.html:37:21
0 some.html:40:25
0 some.html:41:25
420 some.html:40:25
13370 some.html:41:25
840 some.html:40:25
26740 some.html:41:25
1260 some.html:40:25
40110 some.html:41:25
```  
