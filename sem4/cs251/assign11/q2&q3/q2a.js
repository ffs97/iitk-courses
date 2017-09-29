function incrementCount() {
    var counter = document.counterForm.counter;
    if (isNaN(parseInt(counter.value))) {
        counter.value = 0;
    }
    else {
        counter.value = String(parseInt(counter.value) + 1);
    }
}