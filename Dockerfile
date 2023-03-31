#!/bin/bash

FROM ubuntu:22.04

MAINTAINER kevin "kevin@hotmail.com"

WORKDIR /home/terasort

ARG nodename
ENV nodename=$nodename

# error -> ./build/app
COPY ./lib/boost/libboost_json.so.1.79.0 /usr/lib
COPY ./build/app .

RUN apt-get update
RUN apt-get install -y build-essential
# RUN apt-get install -y net-tools
# RUN apt-get install -y iputils-ping
# RUN apt-get install -y telnet

# ENV TZ=Asia/Shanghai
# RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

CMD ["./app","${nodename}"]