# H264SendReceiver
Used to test the functionality of sending and receiving H264 files.

# compile jthread

- only generate static library
- build path: /user_path/H264SendReceiver/thirdPartLib/jthread-1.3.3/build
- install path: /user_path/H264SendReceiver/thirdPartLib/jthread-1.3.3/Install

``` cmake 
 1.cmake -B build -DCMAKE_INSTALL_PREFIX=/user_path/H264SendReceiver/thirdPartLib/jthread-1.3.3/Install -DJTHREAD_COMPILE_STATIC_ONLY=ON
 2.cmake --build build
 3.cmake --install build
```

# compile jrtplib

- only generate static library. depend on jthread
- generate type: Debug
- build path: /user_path/H264SendReceiver/thirdPartLib/jrtplib-3.11.2/build
- install path: /user_path/H264SendReceiver/thirdPartLib/jrtplib-3.11.2/Install

``` cmake
 1.cmake -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=/user_path/h264SendReceiver/thirdPartLib/jrtplib-3.11.2/Install -DJTHREAD_ENABLED=ON -DJThread_DIR=/user_path/h264SendReceiver/thirdPartLib/jthread-1.3.3/Install/lib/cmake/JThread -DJRTPLIB_COMPILE_STATIC=ON
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

# run H264SendReceiver

- note

    - both the local port and the remote port must be even numbers

- run H264Sender

```
 1.cd /user_path/H264SendReceiver/bin
 2./H264Sender sender local_ip local_port remote_ip remote_port send_file_path
```

- run H264Receiver

```
 1.cd /user_path/H264SendReceiver/bin
 2./H264Receiver receiver local_ip local_port remote_ip remote_port receive_file_path
```