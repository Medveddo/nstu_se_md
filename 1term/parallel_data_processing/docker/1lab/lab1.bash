#!/bin/bash

# Получение данных для вывода
curDate=$(date '+%Y-%m-%d %H:%M:%S')
username=$(whoami)
pcName=$(hostname)
processorModel=$(lscpu | grep 'Model name:')
architecture=$(lscpu | grep 'Architecture:')
freqCpu=$(lscpu | grep 'CPU MHz')
coresCount=$(lscpu | grep -v "NUMA" | grep 'CPU(s):')
threadsPerCore=$(lscpu | grep 'Thread(s) per core:')
totalRam=$(cat /proc/meminfo | grep 'MemTotal')
availableRam=$(cat /proc/meminfo | grep 'MemAvailable')
totalHdd=$(lsblk | grep -w "sda" )
LoopZeroHdd=$(lsblk | grep "loop0" )
dfHeader=$(df -aT | grep "Filesystem")
availableShmHdd=$(df -aT | grep "/dev/shm")
availableRootHdd=$(df -aT | grep "/$")
totalSwap=$(cat /proc/meminfo | grep 'SwapTotal')
availableSwap=$(cat /proc/meminfo | grep 'SwapFree')
net_interfaces_count=$(ip -o -4 addr list | grep -c ": ")

echo "Дата: ${curDate} (UTC)"
echo "Имя пользователя: ${username}"
echo "Имя ПК: ${pcName} (id Docker контейнера)"
echo -e "\n[[ ПРОЦЕССОР ]]\n"
echo "  ${processorModel}"
echo "  ${architecture}"
echo "  ${freqCpu}"
echo "  ${coresCount}"
echo "  ${threadsPerCore}"
echo -e "\n[[ ОЗУ ]]\n"
echo "  ${totalRam}"
echo "  ${availableRam}"
echo -e "\n[[ Жёсткий диск ]]\n"
echo -e "  Всего HDD:\t${totalHdd}"
echo -e "  Loop 0:\t${LoopZeroHdd}"
echo ""
echo -e "\n                 \t${dfHeader}"
echo -e "  Доступно root:\t${availableRootHdd}"
echo -e "  Доступно shm:\t\t${availableShmHdd}"
echo -e "\n[[ SWAP ]]\n"
echo "  ${totalSwap}"
echo "  ${availableSwap}"
echo -e "\n[[ Сетевые интерфейсы ]]\n"
echo "  Количество: ${net_interfaces_count}"
echo -e "+---+---------------------+-----------------+--------------------+------------+------------+\n| # |     Интерфейс       |   IP Адрес      |      MAC-адрес     |  Загрузка  | Скачивание |\n+---+---------------------+-----------------+--------------------+------------+------------+"
counter=0
for iface in $(ip -o -4 addr list | awk '{print $2}' | tr '\n' ' ')
do
    counter=$[$counter + 1]
    ipaddr=$(ip -o -4 addr list $iface | awk '{print $4}' | cut -d/ -f1)
    mac=$(ip address show ${iface} | grep -oP '(?<=link/ether\s)\K[\da-f:]+|(?<=link/loopback\s)\K[\da-f:]+')
    speedtest_result=$(speedtest --simple --secure --source=${ipaddr} 2> /dev/null)
    upload=$(grep -oP 'Upload: \K.*\s' <<< $speedtest_result) 
    download=$(grep -oP 'Download: \K.*\s' <<< $speedtest_result) 
    printf "|%3s|%20s | %-16s| %18s | %10s | %10s |\n" $counter $iface $ipaddr $mac $upload $download

done
echo "+---+---------------------+-----------------+--------------------+------------+------------+"
