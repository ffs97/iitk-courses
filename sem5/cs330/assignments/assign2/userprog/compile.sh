cd ../test
make

echo -e "\n\n\n\n\n"

cd ../threads
make depend
echo -e "\n\n"
make

echo -e "\n\n\n\n\n"

cd ../userprog
make depend
echo -e "\n\n"
make

echo -e "\n\n"