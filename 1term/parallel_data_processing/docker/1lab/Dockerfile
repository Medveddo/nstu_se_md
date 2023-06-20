FROM debian:11-slim

RUN apt update && apt install iproute2 speedtest-cli -y

COPY lab1.bash lab1.bash

RUN useradd --user-group --system --create-home --no-log-init charushindd
USER charushindd

# CMD sleep 300 # для отладки
CMD bash lab1.bash
