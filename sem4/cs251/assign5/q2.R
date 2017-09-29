data = read.csv("./leaves.csv")

x = nrow(data)
data = na.omit(data)
data = data[!(is.na(data$Species) | data$Species==""), ]
data = data[!(is.na(data$Good) | data$Good==""), ]

if (! is.numeric(data[, 'Sepal.Length']) || ! is.numeric(data[, 'Sepal.Width']) || ! is.numeric(data[, 'Petal.Length']) || ! is.numeric(data[, 'Petal.Width'])
	|| ! is.factor(data[, 'Species']) || ! is.factor(data[, 'Good']) || ! (ncol(data) == 6)) {
	cat("Invalid Input Format")
	quit()
}
if (nrow(data) < x) {
	cat("Warning: Inputs with empty or NULL identifier have been removed\n\n")
}

Species = as.character(unique(data[, 'Species']))
Good = as.character(unique(data[, 'Good']))
Attributes = c("Sepal.Length", "Sepal.Width", "Petal.Length", "Petal.Width")


cat("Mean for Different Species\n\n")
aggregate(cbind(Sepal.Length, Sepal.Width, Petal.Length, Petal.Width)~Species, data, mean)

cat("\n\nMean for Different Goodness\n\n")
aggregate(cbind(Sepal.Length, Sepal.Width, Petal.Length, Petal.Width)~Good, data, mean)

cat("\n\nContingency Table\n")
table(data[['Species']], data[['Good']])

mean.df = aggregate(cbind(Sepal.Length, Sepal.Width, Petal.Length, Petal.Width)~Species+Good, data, mean)
print(as.data.frame(t(mean.df)))