#!/bin/bash

# FROM alpine:3.13.6
# FROM alpine # exec ./app: no such file or directory
FROM ubuntu:22.04
# FROM gcc

MAINTAINER kevin "kevin@hotmail.com"

WORKDIR /home/terasort

ARG node
# ENV node=${node}

# COPY中不能使用这种路径：./build/app
COPY ./lib/boost/libboost_json.so.1.79.0 /usr/lib
COPY ./build/app .
COPY ./test/node_conf.json .

RUN apt-get update
RUN apt-get install -y build-essential
# RUN apt-get install -y cmake

# COPY ./* .
# RUN cmake -B build
# RUN cmake --build build

# COPY ./test.c .
# RUN gcc -o testapp test.c

# RUN mkdir -p /usr/lib/x86_64-linux-gnu
# COPY ./lib/std/* /usr/lib/x86_64-linux-gnu

# ENV TZ=Asia/Shanghai
# RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

CMD ["./app",${node}]
# CMD ["cmake","--version"]
# CMD ["./testapp"]

# CMD ["./build/app","node2"]


# https://cloud.tencent.com/developer/article/1632733