#!/bin/bash

FROM ubuntu:22.04

MAINTAINER kevin "kevin@hotmail.com"

WORKDIR /home/dscproducer

ARG nodename
ENV nodename=$nodename

# error -> ./build/app
# COPY ./lib/boost/libboost_json.so.1.79.0 /usr/lib

RUN apt-get update
RUN apt-get install -y build-essential
RUN apt-get install -y openssl
RUN apt-get install -y libssl-dev
# RUN apt-get install -y net-tools
# RUN apt-get install -y iputils-ping
# RUN apt-get install -y telnet

COPY ./dscproducer .
COPY ./config ./config
COPY ./data ./data

COPY ./librdkafka /usr/lib

# ENV TZ=Asia/Shanghai
# RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

CMD ["ls","/usr/local/lib"]
CMD ["./dscproducer"]