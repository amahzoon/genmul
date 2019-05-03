#!/bin/bash








L=`ls *.cpp *.hpp | sort`


S1=`sha256sum $L > ckey`
S2=`sha256sum ckey > ckey2`

echo ----------------------------------------------------------
cat ckey
echo ----------------------------------------------------------
cat ckey2

