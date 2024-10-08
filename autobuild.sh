#!/bin/bash

set -e

# 如果没有build目录，创建build目录
if [ ! -d `pwd`/build ]; then
    mkdir `pwd`/build
fi

rm -rf `pwd`/build/*

cd `pwd`/build &&
    cmake .. &&
    make

# 回到项目根目录
cd ..

# 把头文件拷贝到 /usr/include/MuduoServer so库拷贝到/usr/lib
if [ ! -d /usr/include/MuduoServer ]; then  
    mkdir /usr/include/MuduoServer
fi

for header in `ls *.h`
do
    cp $header /usr/include/MuduoServer
done

cp `pwd`/lib/libMuduoServer.so /usr/lib

ldconfig
