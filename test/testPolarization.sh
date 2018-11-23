#! /bin/bash
# 

rm -f testPolarization.cout 
./testPolarization

if [ "$?" -ne 0 ]
then
  exit 1;
fi

if [ ! -e testPolarization.cout ]
then
   echo "ERROR: testPolarization.cout does not exist"
   exit 1;
fi

OS=`uname`
case "$OS" in
CYGWIN*)
  cmd2=` testPolarization4.out testPolarization5.out`
  cmd3=` testPolarization1.dat testPolarization2.dat`
  ;;
*)
  cmd2=` testPolarization4.out testPolarization5.out`
  cmd3=` testPolarization1.dat testPolarization2.dat`
esac

if [ -n "$cmd2" ]
then
  echo $cmd2
  exit 1;
fi

if [ -z "$cmd3" ]
then
  echo "we expect testPolarization1.dat testPolarization2.dat to differ"
  exit 1;
fi

exit 0;
