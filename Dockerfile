#!/bin/bash

FROM ubuntu:22.04

MAINTAINER kevin "kevin@hotmail.com"

WORKDIR /home/dsc

ARG nodename
ENV nodename=$nodename

# error -> ./build/app
COPY ./lib/boost/libboost_json.so.1.79.0 /usr/lib
COPY ./lib/librdkafka/* /usr/lib
# COPY ./build/app .

RUN apt-get update
RUN apt-get install -y build-essential
RUN apt install -y curl libssl-dev libcurl4-openssl-dev libcurl4 openssl libssl-dev
RUN apt install -y libcurl4-gnutls-dev
RUN apt-get install -y net-tools iputils-ping telnet

# RUN ln -s /usr/lib/x86_64-linux-gnu/libcurl-gnutls.so.4.7.0 /usr/lib/x86_64-linux-gnu/libcurl-gnutls.so.4

# ENV TZ=Asia/Shanghai
# RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

CMD ["./app","${nodename}"]