library(plyr)

setwd("/home/herb/git/networks")

data = read.csv("caveman-stats.csv")

names(data) <- c("edges", "length")


#data = ddply(data, c("edges"), function(df)c(max(df$length)))

names(data) <- c("edges", "length")


plot(data$edges, data$length, ylab="Length of Infection", xlab="Number of Edges", main="Disease Persistence vs. Number of Edges Added in Random Graph", pch=".")



data = read.csv("preferential-stats.csv")

names(data) <- c("edges", "length")


#data = ddply(data, c("edges"), function(df)c(mean(df$length)))

names(data) <- c("edges", "length")

plot(data$edges, data$length, ylab="Length of Infectoittn", xlab="Number of Edges", main="Disease Persistence vs. Number of Edges Added in Preferential Attachment Graph", pch=".")