libra2

library(animation)
library(ggplot2)

ani.start()

for(i in seq(1, 40, by=1)) {
  if(i < 10) {
    file = paste("stats-0.00", i, "000", sep="")
  }
  else {
    file = paste("stats-0.0", i, "000", sep="")  
  }
  data = read.csv(paste("/home/herb/git/networks/data/dataset-6/", file, ".csv", sep=""), sep=",")
  
  p = ggplot(data, aes(x=p, y=k, z=times_sick))
  p = p + geom_tile(aes(fill=times_sick), color="white") + scale_fill_gradient(low="white", high="black", limit=c(0, 8))
  p = p + opts(title = (i/1000))
  
  print(p)
}

ani.stop()