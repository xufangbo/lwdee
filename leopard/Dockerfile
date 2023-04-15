#!/bin/bash

FROM ubuntu:22.04

MAINTAINER kevin "kevin@hotmail.com"

WORKDIR /home/leopard

ARG nodename
ENV nodename=$nodename

# error -> ./build/app
# COPY ./app .
# COPY ./config ./config

RUN rm -rf /etc/apt/sources.list
COPY ./sources.list /etc/apt
RUN apt-get update
# RUN apt-get install -y gcc
# RUN apt-get install -y g++
RUN apt-get install -y build-essential

RUN apt-get install -y net-tools
RUN apt-get install -y iputils-ping
RUN apt-get install -y telnet

# ENV TZ=Asia/Shanghai
# RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

CMD ["./app"]