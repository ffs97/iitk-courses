tput reset

cd ../bin
make 

cd ../test
make

echo -e "\n\n\n\n\n"

cd ../threads
# make depend
# echo -e "\n\n"
make

echo -e "\n\n\n\n\n"

cd ../userprog
# make depend
# echo -e "\n\n"
make

echo -e "\n\n" 

./nachos -rs 0 -A 3 -R 1 -x ../test/$1