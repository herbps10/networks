library(plyr)
library(ggplot2)

data = read.table("/home/herb/git/networks/data/watts-myway.csv", header=T, sep=",")

data = ddply(data, c("p", "k"), function(df)c(mean(df$R0), mean(df$infected)))
  
names(data) <- c("p", "k", "R0", "infected")

plot(data$k, data$infected, xlab="K", ylab="R0", main="Experimental Infected versus K, T = 0.02", xlim=c(0, 60))
abline(h=1, lty=3)

ggplot(data, aes(x = k, y = infected, colour = p)) + geom_point()


p = ggplot(data, aes(x=p, y=k, z=infected))
p = p + geom_tile(aes(fill=infected), color="white") + scale_fill_gradient(low="white", high="black")
p = p + opts(title = "")
print(p)
#p = ggplot(data, aes(x=p, y=k, z=R0))
#p = p + geom_tile(aes(fill=R0), color="white") + scale_fill_gradient(low="white", high="black")
#p = p + opts(title = i)
#print(p)