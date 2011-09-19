library(igraph)
library(animation)

setwd("/home/herb/git/networks/")

ani.start()

g = read.graph("/home/herb/git/networks/graph-data/graph1-k-1-p-0.500000.graphml", "graphml")
#l = layout.fruchterman.reingold(g)
l = layout.circle(g)

for(i in seq(1, 100)) {
  g = read.graph(paste("/home/herb/git/networks/graph-data/graph", i, "-k-1-p-0.500000.graphml", sep=""), "graphml")
  
  plot(g, layout=l, vertex.label="", vertex.size=5);
}

ani.stop()