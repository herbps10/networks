  library(igraph)
library(animation)
library(scatterplot3d)

ani.start()
for(R0 in 1:40) {
  print(R0)
  
  data = matrix(nrow = 0, ncol = 3, dimnames = list(c(), c("p", "k", "T")))
  
  for(p in seq(0, 1, by = 0.1)) {
    for(k in seq(1, 40, by = 1)) {    
      g = watts.strogatz.game(1, 100, k, p)
      
      t = numeric(0)
      for(v in V(g)) {
        t[v] = 1 - (1 - ((R0/degree(g)[v]) / 3))
      }
      
      m = mean(t)
      if(m == Inf)
      {
        m = 1
      }
      
      data = rbind(data, c(p, k, m))
    }
  }
  
  #plot3d(data[,1], data[,2], data[,3])
  scatterplot3d(data[,1], data[,2], data[,3], xlab="p", ylab="k", zlab="T", angle=45, zlim=c(0, 4), main=R0)
}

ani.stop()