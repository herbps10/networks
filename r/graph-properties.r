library(igraph)

data = data.frame()

k=2
for(p in seq(0, 1, by=0.01)) {
  for(k in seq(2, 40, by=1)) {
    g = watts.strogatz.game(1, size=1000, nei=k, p=p)
    
    data = rbind(data, c(p, k, average.path.length(g), transitivity(g)))
  }
}

colnames(data) <- c("p", "k", "average.path.length", "clustering.coefficient")

plot(data$p, data$average.path.length, type='p', xlab='Rewiring Probability (P)', ylab="Average Path Length", ylim=c(0, 20))