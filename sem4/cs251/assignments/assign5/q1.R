load("./data.txt")

qqnorm(data1)
qqnorm(data2)
qqnorm(data3)

if (is.na(data1) || is.na(data2) || is.na(data3)) {
	cat("The data includes NULL Values")
	quit()
}

if (! is.numeric(data1) || ! is.numeric(data2) || ! is.numeric(data3)) {
	cat("Invalid Data Format")
	quit()
}

data.table = data.frame(
	
	data_variable = c("Data 1", "Data 2", "Data 3"),

	mean = c(mean(data1, na.rm = TRUE), mean(data2, na.rm = TRUE), mean(data3, na.rm = TRUE)),

	median = c(median(data1, na.rm = TRUE), median(data2, na.rm = TRUE), median(data3, na.rm = TRUE)),

	standard_deviation = c(sd(data1, na.rm = TRUE), sd(data2, na.rm = TRUE), sd(data3, na.rm = TRUE))

)

quantile.table = data.frame(
	
	data1 = quantile(data1),
	data2 = quantile(data1),
	data3 = quantile(data1)

)

print(data.table)
cat("\nQuantiles:\n")
print(quantile.table)