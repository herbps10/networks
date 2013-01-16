library(igraph)
library(animation)

g = read.graph("/home/herb/git/networks/data/graph-1.net", format="pajek")
l = layout.circle(g)

ani.options(ani.height = 1000, ani.width = 1000)

ani.start()

for(i in 1:104) {
  g = read.graph(paste("/home/herb/git/networks/data/graph-", i, ".net", sep=""), format="pajek")
  
  plot(g, layout=l, vertex.size=2, vertex.label="", edge.color="white")
}

ani.stop()