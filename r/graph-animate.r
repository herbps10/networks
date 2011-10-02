library(igraph)
library(animation)

setwd("/home/herb/git/networks/")

ani.start()

g = read.graph("/home/herb/git/networks/graph-data/graph1.pajek", "pajek")
#l = layout.fruchterman.reingold(g)
l = layout.circle(g)

for(i in seq(1, 100)) {
  print(i)
  g = read.graph(paste("/home/herb/git/networks/graph-data/graph", i, ".pajek", sep=""), "pajek")
  
  plot(g, layout=l, vertex.label="", vertex.size=5);
}

ani.stop()