library(plyr)

data = read.table("/home/herb/git/networks/data/infectious_counts.csv", header=T, sep=",")

counts = count(data$count)

plot(counts)