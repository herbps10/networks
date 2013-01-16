library(igraph)
library(animation)

ani.start()

for(nei in seq(2, 10)) {
  nei = 2
  data = data.frame()
  for(p in seq(0, 1, by=0.01)) {
    g = watts.strogatz.game(dim = 1, size=100, nei=nei, p=p)
    
    data = rbind(data, c(nei, p, average.path.length(g)))
  }
  
  names(data) <- c("nei", "p", "path.length")

  plot(data$p, data$path.length, ylim=c(0, 25))
}
  
ani.stop()