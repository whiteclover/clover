#!/usr/bin/bash

for f in `ls pb` 
do
	 protoc -I=./pb --cpp_out=.  ./pb/$f;
	 rename 's/.cc$/.cpp/' *.cc
done



# for f in `ls pb` 
# do
# 	 protoc -I=./pb --python_out=.  ./pb/$f;
# done