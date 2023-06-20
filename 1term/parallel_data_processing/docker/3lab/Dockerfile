FROM debian:10-slim
WORKDIR /usr/src/app
RUN apt-get update 
RUN apt-get install g++ -y 
RUN apt-get install linux-perf -y
RUN cp /usr/bin/perf_4.19 /usr/bin/perf_5.4
COPY 3lab.cpp 3lab.cpp
COPY 3lab.bash 3lab.bash
CMD bash 3lab.bash
