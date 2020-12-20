#!/bin/bash

cd "$( dirname "${BASH_SOURCE[0]}" )"

trans='./../../build-trans-*/trans'

cd petlje

clang whilefor.c
./a.out > output.txt
rm a.out

$trans whilefor.c samodo.c do
$trans whilefor.c samofor.c for
$trans whilefor.c samowhile.c while

$trans samodo.c samodo2.c do
$trans samodo.c samofor2.c for
$trans samodo.c samowhile2.c while

cd ../uslovi

clang ifswitch.c
./a.out > output.txt
rm a.out

$trans ifswitch.c samoif.c if
$trans ifswitch.c samoswitch.c switch

$trans samoif.c samoswitch2.c switch
$trans samoswitch.c samoif2.c if

cd ../rekurzija

clang rekiter.c
./a.out > output.txt
rm a.out

$trans rekiter.c samoiter.c iter
$trans rekiter.c samorek.c rek

