#!/bin/bash
# date: 2017-09-11
# author: JiaoLinfeng

srm_path=$SRM_PATH
if [ -z ${srm_path} ]; then
    srm_path=$HOME/garbage
fi
echo "clean ${srm_path} ..."
# determine if target path exists
if [ ! -d ${srm_path} ]; then
    echo "${srm_path} not exists"
    exit 1;
fi
find ${srm_path} -maxdepth 1 -type d -atime +20 -exec rm -rf {} \; > /dev/null 2>&1
echo "clean done!"
