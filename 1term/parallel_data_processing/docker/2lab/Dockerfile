FROM debian:11-slim

WORKDIR /usr/src/app

RUN apt update && apt install gcc g++ -y

COPY lab2.cpp lab2.cpp
COPY lab2.bash lab2.bash

# CMD sleep 600 # for debug
CMD bash lab2.bash
