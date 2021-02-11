#!/bin/bash

cd "$( dirname "${BASH_SOURCE[0]}" )"

trans='./../../build-trans-*/trans'
clang='clang-11 -O0'

### ODMOTAVANJE #####################
cd ../odmot

$clang jednostruke.c
./a.out > output.txt
rm a.out

rm bezpromene.c 2> /dev/null
$trans jednostruke.c bezpromene.c o0

$clang bezpromene.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

rm trostruke.c 2> /dev/null
$trans jednostruke.c trostruke.c o2

$clang trostruke.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

#####################################

### USLOVI ##########################
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
#####################################

### REKURZIJA #######################
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

$clang samorek2.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt

rm samoiter2.c 2> /dev/null
$trans samorek.c samoiter2.c iter

$clang samoiter2.c
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt
#####################################

### UMETANJE ########################
cd ../umetanje

$clang normal.c
./a.out > output.txt
rm a.out

rm bloated.c 2> /dev/null
$trans normal.c bloated.c u3

$clang bloated.c 2> /dev/null
./a.out > output2.txt
rm a.out

diff output.txt output2.txt -q
rm output2.txt
#####################################

