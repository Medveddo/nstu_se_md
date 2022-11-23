#!/bin/bash

# to run it
# chmod u+x sysinfo.bash 

# Prints date
cur_date=$(date '+%Y-%m-%d %H:%M:%S')
echo "Date: ${cur_date}"

# Username
username=$USER
echo "Username: ${username}"

# PC name
pc_name=$(hostname)
echo "PC name: ${pc_name}"

# Processor info
echo -e "\n-- Processor --\n"

processor_model=$(lscpu | grep 'Model name:')
echo "  ${processor_model}"

architecture=$(lscpu | grep 'Architecture:')
echo "  ${architecture}"

freq_cpu=$(lscpu | grep 'CPU MHz')
freq_cpu_max=$(lscpu | grep 'CPU max MHz')
freq_cpu_min=$(lscpu | grep 'CPU min MHz')
echo "  ${freq_cpu}"
echo "  ${freq_cpu_min}"
echo "  ${freq_cpu_max}"

cores_count=$(lscpu | grep -v "NUMA" | grep 'CPU(s):')
threads_per_core=$(lscpu | grep 'Thread(s) per core:')
echo "  ${cores_count}"
echo "  ${threads_per_core}"

# Memory
echo -e "\n-- RAM --\n"

total_ram=$(cat /proc/meminfo | grep 'MemTotal')
available_ram=$(cat /proc/meminfo | grep 'MemAvailable')
echo "  ${total_ram}"
echo "  ${available_ram}"

echo -e "\n-- Hard Drive --\n"
total_hdd=$(lsblk | grep -w "sda" )
root_mounted_hdd=$(lsblk | grep "part /$" )

echo -e "  Total HDD:\t${total_hdd}"
echo -e "  Root mounted:\t${root_mounted_hdd}"
echo ""
df_header=$(df -aT | grep "Filesystem")
available_home_hdd=$(df -aT | grep "/home")
available_root_hdd=$(df -aT | grep "/$")
echo -e "\n                 \t${df_header}"
echo -e "  Available root:\t${available_root_hdd}"
echo -e "  Available home:\t${available_home_hdd}"

echo -e "\n-- SWAP --\n"

total_swap=$(cat /proc/meminfo | grep 'SwapTotal')
available_swap=$(cat /proc/meminfo | grep 'SwapFree')
echo "  ${total_swap}"
echo "  ${available_swap}"

echo -e "\n-- Newtork --\n"

net_interfaces_count=$(ip -o -4 addr list | grep -c ": ")

echo "  Network interfaces count: ${net_interfaces_count}"


echo -e "+---+---------------------+-----------------+--------------------+------------+------------+\n| # |     Interface       | IP Address      |    MAC-address     |   Upload   |  Download  |\n+---+---------------------+-----------------+--------------------+------------+------------+"

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

# Speed ?
# https://serverfault.com/questions/586322/how-to-find-out-capacity-for-network-interfaces

