library(ggplot2)
library(animation)

setwd("/home/herb/git/networks/data/");

ani.start()

for(i in seq(0, 40, by=1)) {
  if(i < 10) {
    file = paste("stats-", i, sep="")
  }
  else {
    file = paste("stats-", i, sep="")  
  }
  
  data = read.table(paste("/home/herb/git/networks/data/dataset-3/", file, ".csv", sep=""), header=T, sep=",")
  names(data) <- c("t", "p", "k", "repitition", "day")
  simple = data.frame(T=data$t,K=data$k,p=data$p,length=data$day)
  
  p = ggplot(simple, aes(x=p, y=K, z=length))
  p = p + geom_tile(aes(fill=length), color="white") + scale_fill_gradient(low="white", high="black", limit=c(0, 1500))
  p = p + opts(title = i)
  
  print(p)
}

ani.stop()