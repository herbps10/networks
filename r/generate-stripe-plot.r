library(ggplot2)
library(animation)
library(plyr)

setwd("/home/herb/git/networks/data/dataset-1");

ani.start()

for(i in seq(0, 99, by=1)) {
  print(i)
  
  if(i < 10) {
    file = paste("stats-0.00", i, "000", sep="")
  }
  else if(i >= 10 && i < 100) {
    file = paste("stats-0.0", i, "000", sep="")  
  }
  else {
    file = paste("stats-0.", i, "0000", sep="")
  }
  
  data = read.table(paste("/home/herb/git/networks/data/", file, ".csv", sep=""), header=T, sep=",")
  
  data = ddply(data, c("p", "k"), function(df)c(mean(df$day),mean(df$times_sick)))
  
  names(data) <- c("p", "k", "day", "times_sick")
  
  p = ggplot(data, aes(x=p, y=k, z=day))
  p = p + geom_tile(aes(fill=day), color="white") + scale_fill_gradient(low="white", high="black", limit=c(0, 1400))
  p = p + opts(title = i)
  print(p)
}

ani.stop()