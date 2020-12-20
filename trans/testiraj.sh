#!/bin/bash

cd "$( dirname "${BASH_SOURCE[0]}" )"

trans='./../../build-trans-*/trans'
clang='clang-11'

cd petlje

$clang whilefor.c
./a.out > output.txt
rm a.out

$trans whilefor.c samodo.c do

$clang samodo.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

$trans whilefor.c samofor.c for

$clang samofor.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

$trans whilefor.c samowhile.c while

$clang samowhile.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

$trans samodo.c samodo2.c do

$clang samodo2.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

$trans samodo.c samofor2.c for

$clang samofor2.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

$trans samodo.c samowhile2.c while

$clang samowhile2.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

cd ../uslovi

$clang ifswitch.c
./a.out > output.txt
rm a.out

$trans ifswitch.c samoif.c if

$clang samoif.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

$trans ifswitch.c samoswitch.c switch

$clang samoswitch.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

$trans samoif.c samoswitch2.c switch

$clang samoswitch2.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

$trans samoswitch.c samoif2.c if

$clang samoif2.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

cd ../rekurzija

$clang rekiter.c
./a.out > output.txt
rm a.out

$trans rekiter.c samoiter.c iter

$clang samoiter.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

$trans rekiter.c samorek.c rek

$clang samorek.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

$trans samoiter.c samorek2.c rek

clang samorek2.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

$trans samorek.c samoiter2.c iter

clang samoiter2.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

