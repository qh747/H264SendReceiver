# H264SendReceiver
Used for testing the functionality of sending and receiving H264 files.

# compile jthread

- only generate static library
- build path: /user_path/H264SendReceiver/3rdPart/jthread-1.3.3/build
- install path: /user_path/H264SendReceiver/3rdPart/jthread-1.3.3/Install

``` cmake 
 1.cmake -B build -DCMAKE_INSTALL_PREFIX=/user_path/H264SendReceiver/3rdPart/jthread-1.3.3/Install -DJTHREAD_COMPILE_STATIC_ONLY=ON
 2.cmake --build build
 3.cmake --install build
```

# compile jrtplib

- only generate static library. depend on jthread
- generate type: Debug
- build path: /user_path/H264SendReceiver/3rdPart/jrtplib-3.11.2/build
- install path: /user_path/H264SendReceiver/3rdPart/jrtplib-3.11.2/Install

``` cmake
 1.cmake -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/user_path/h264SendReceiver/3rdPart/jrtplib-3.11.2/Install -DJTHREAD_ENABLED=ON -DJThread_DIR=/user_path/h264SendReceiver/3rdPart/jthread-1.3.3/Install/lib/cmake/JThread -DJRTPLIB_COMPILE_STATIC=ON
 2.cmake --build build
 3.cmake --install build
```

# compile H264SendReceiver

- build path: /user_path/H264SendReceiver/build
- bin path: /user_path/H264SendReceiver/bin

``` cmake
 1.cmake -B build 
 2.cmake --build build
```
