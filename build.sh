#!/bin/bash
cmake -B cmake_build
cmake --build cmake_build -- -j $(nproc) 