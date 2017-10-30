for i in {1..10..1}; do
    echo -e "Scheduler $i\n"
    for file in batches/batch{1..4..1}.txt; do
        echo $i > t
        cat $file >> t

        ./nachos -F t | grep -E 'CPU Util|waiting in' | sed 's/^/\t/g'
    done
    echo -e "\n"
done