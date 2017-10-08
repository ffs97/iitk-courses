#!/bin/sh 

rm -f .license

cat license.txt

printf "Do you wish to continue ? "

if [ `uname` = Linux ]; then
echo 
fi

answer=none

while [ $answer = none ]; do
  read a
  if [ "$a" = "yes" -o "$a" = "Yes" ]; then
     answer=yes
  fi
  if [ "$a" = "no"  -o "$a" = "No"  ]; then
     answer=no
  fi
  if [ $answer = none ]; then
    printf "\nPlease type yes or no : "
    if [ `uname` = Linux ]; then
      echo 
    fi
  fi
done


if [ $answer = no ]; then
   echo " "
   echo "Installation stopped."
   echo " "
else
   echo " "
   echo "Installation continues ..."
   echo " "
   touch .license
fi

