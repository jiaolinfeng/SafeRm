#!/bin/bash
# author: JiaoLinfeng
# date  : 2017-09-07

function LOG_INFO()
{
    echo -e "\033[32m[INFO] \033[0m$1"
}

function LOG_ERROR()
{
    echo -e "\033[31m[ERROR] \033[0m$1"
}

source_file="srm.cpp"
target_folder="${HOME}/bin"
target="srm"
CC="g++"

#decide whether the source file exists
if [ ! -f $source_file ]; then
    LOG_ERROR "Source File is Not Found"
    exit 1
fi

#compile
LOG_INFO "Compiling..."
${CC} -O2 -o ${target} ${source_file}
if [ $? -ne 0 ];then
    LOG_ERROR "Compiled Failed"
    exit 2
fi
LOG_INFO "Compiled Successfully"

#install
LOG_INFO "Installing..."
mkdir -p ${target_folder}
if [ $? -ne 0 ]; then
    LOG_ERROR "Create Target Folder Failed"
    exit 2
fi
mv ${target} ${target_folder}
LOG_INFO "Installed Successfully"
