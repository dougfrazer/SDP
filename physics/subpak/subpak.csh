#!/bin/csh
#
cp subpak.H /$HOME/include
#
g++ -c -g -I /$HOME/include subpak.C >& compiler.txt
if ( $status != 0 ) then
  echo "Errors compiling subpak.C."
  exit
endif
rm compiler.txt
#
mv subpak.o ~/libcpp/$ARCH/subpak.o
#
echo "Library installed as ~/libcpp/$ARCH/subpak.o"
