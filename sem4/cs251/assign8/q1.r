
#DISTRIBUTION CODE FOR QUESTION 2

distributions.norm = 0
distributions.unif = 0

frequency.norm = matrix(
	c(
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	), nrow=4, ncol=10, byrow=TRUE)

frequency.unif = matrix(
	c(
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	), nrow=4, ncol=10, byrow=TRUE)

for (i in c(1:10)) {

	#FOR NORMAL DISTRIBUTION

	dist = rnorm(100, 0.5, 0.15)
	
	filename = paste(paste("distributions/dist_norm",i,sep="_"), "txt", sep=".")
	cat("#dist", dist, file=filename, sep="\n")

	distributions.norm = rbind(distributions.norm, dist)

	frequency.norm[1, i] = length(dist[dist < 0.25])
	dist = dist[dist >= 0.25]
	frequency.norm[2, i] = length(dist[dist < 0.50])
	dist = dist[dist >= 0.50]
	frequency.norm[3, i] = length(dist[dist < 0.75])
	dist = dist[dist >= 0.75]
	frequency.norm[4, i] = length(dist)

	#FOR UNIFORM DISTRIBUTION

	dist = runif(100, 0.0, 1.0)
	
	filename = paste(paste("distributions/dist_unif",i,sep="_"), "txt", sep=".")
	cat("#dist", dist, file=filename, sep="\n")

	distributions.unif = rbind(distributions.unif, dist)

	frequency.unif[1, i] = length(dist[dist < 0.25])
	dist = dist[dist >= 0.25]
	frequency.unif[2, i] = length(dist[dist < 0.50])
	dist = dist[dist >= 0.50]
	frequency.unif[3, i] = length(dist[dist < 0.75])
	dist = dist[dist >= 0.75]
	frequency.unif[4, i] = length(dist)
}

distributions.norm = distributions.norm[2:11,]
distributions.unif = distributions.unif[2:11,]

cat(
	paste("0.00", mean(frequency.norm[1,]), min(frequency.norm[1,]), max(frequency.norm[1,])),
	paste("0.25", mean(frequency.norm[2,]), min(frequency.norm[2,]), max(frequency.norm[2,])),
	paste("0.50", mean(frequency.norm[3,]), min(frequency.norm[3,]), max(frequency.norm[3,])),
	paste("0.75", mean(frequency.norm[4,]), min(frequency.norm[4,]), max(frequency.norm[4,])),
	paste("1.00", 0, 0, 0),
	file="distributions/dist_norm_complete_1.txt", sep="\n")

cat(
	paste("0.00", mean(frequency.unif[1,]), min(frequency.unif[1,]), max(frequency.unif[1,])),
	paste("0.25", mean(frequency.unif[2,]), min(frequency.unif[2,]), max(frequency.unif[2,])),
	paste("0.50", mean(frequency.unif[3,]), min(frequency.unif[3,]), max(frequency.unif[3,])),
	paste("0.75", mean(frequency.unif[4,]), min(frequency.unif[4,]), max(frequency.unif[4,])),
	paste("1.00", 0, 0, 0),
	file="distributions/dist_unif_complete_1.txt", sep="\n")


#DISTRIBUTION CODE FOR QUESTION 3


frequency.norm = matrix(
	c(
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	), nrow=3, ncol=10, byrow=TRUE)

frequency.unif = matrix(
	c(
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	), nrow=3, ncol=10, byrow=TRUE)

for (i in c(1:10)) {

	#FOR NORMAL DISTRIBUTION

	dist = distributions.norm[i,]
	
	frequency.norm[1, i] = length(dist[dist <= 0.34])
	dist = dist[dist > 0.34]
	frequency.norm[2, i] = length(dist[dist < 0.66])
	dist = dist[dist >= 0.66]
	frequency.norm[3, i] = length(dist)

	#FOR UNIFORM DISTRIBUTION

	dist = distributions.unif[i,]
	
	frequency.unif[1, i] = length(dist[dist <= 0.34])
	dist = dist[dist > 0.34]
	frequency.unif[2, i] = length(dist[dist < 0.66])
	dist = dist[dist >= 0.66]
	frequency.unif[3, i] = length(dist)
}

cat(
	paste("A",
		mean(frequency.norm[1,1:5]), min(frequency.norm[1,1:5]), max(frequency.norm[1,1:5]),
		mean(frequency.norm[1,6:10]), min(frequency.norm[1,6:10]), max(frequency.norm[1,6:10])),
	paste("B",
		mean(frequency.norm[2,1:5]), min(frequency.norm[2,1:5]), max(frequency.norm[2,1:5]),
		mean(frequency.norm[2,6:10]), min(frequency.norm[2,6:10]), max(frequency.norm[2,6:10])),
	paste("C",
		mean(frequency.norm[3,1:5]), min(frequency.norm[3,1:5]), max(frequency.norm[3,1:5]),
		mean(frequency.norm[3,6:10]), min(frequency.norm[3,6:10]), max(frequency.norm[3,6:10])),
	file="distributions/dist_norm_complete_2.txt", sep="\n")

cat(
	paste("A",
		mean(frequency.unif[1,1:5]), min(frequency.unif[1,1:5]), max(frequency.unif[1,1:5]),
		mean(frequency.unif[1,6:10]), min(frequency.unif[1,6:10]), max(frequency.unif[1,6:10])),
	paste("B",
		mean(frequency.unif[2,1:5]), min(frequency.unif[2,1:5]), max(frequency.unif[2,1:5]),
		mean(frequency.unif[2,6:10]), min(frequency.unif[2,6:10]), max(frequency.unif[2,6:10])),
	paste("C",
		mean(frequency.unif[3,1:5]), min(frequency.unif[3,1:5]), max(frequency.unif[3,1:5]),
		mean(frequency.unif[3,6:10]), min(frequency.unif[3,6:10]), max(frequency.unif[3,6:10])),
	file="distributions/dist_unif_complete_2.txt", sep="\n")