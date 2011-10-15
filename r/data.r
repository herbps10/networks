library(ggplot2)

setwd("/home/herb/git/networks/data/");

data = read.table("data-t-90.csv", header=T, sep=",")

simple = data.frame(K=data$k,p=data$p,length=data$day)

p = ggplot(simple, aes(x=p, y=K, z=length))
p + geom_tile(aes(fill=length))