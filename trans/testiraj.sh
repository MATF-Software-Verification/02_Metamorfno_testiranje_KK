#!/bin/bash

cd "$( dirname "${BASH_SOURCE[0]}" )"

trans='./../../build-trans-*/trans'
clang='clang-11'

cd petlje

$clang whilefor.c
./a.out > output.txt
rm a.out

rm samodo.c 2> /dev/null
$trans whilefor.c samodo.c do

$clang samodo.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

rm samofor.c 2> /dev/null
$trans whilefor.c samofor.c for

$clang samofor.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

rm samowhile.c 2> /dev/null
$trans whilefor.c samowhile.c while

$clang samowhile.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

rm samodo2.c 2> /dev/null
$trans samodo.c samodo2.c do

$clang samodo2.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

rm samofor2.c 2> /dev/null
$trans samodo.c samofor2.c for

$clang samofor2.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

rm samowhile2.c 2> /dev/null
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

rm samoif.c 2> /dev/null
$trans ifswitch.c samoif.c if

$clang samoif.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

rm samoswitch.c 2> /dev/null
$trans ifswitch.c samoswitch.c switch

$clang samoswitch.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

rm samoswitch2.c 2> /dev/null
$trans samoif.c samoswitch2.c switch

$clang samoswitch2.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

rm samoif2.c 2> /dev/null
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

rm samoiter.c 2> /dev/null
$trans rekiter.c samoiter.c iter

$clang samoiter.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

rm samorek.c 2> /dev/null
$trans rekiter.c samorek.c rek

$clang samorek.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

rm samorek2.c 2> /dev/null
$trans samoiter.c samorek2.c rek

clang samorek2.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

rm samoiter2.c 2> /dev/null
$trans samorek.c samoiter2.c iter

clang samoiter2.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

