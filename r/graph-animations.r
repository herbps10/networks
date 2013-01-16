library(ggplot2)
library(plyr)
library(gtools)
library(colorspace)
library(animation)

all_data = read.table("/home/herb/git/networks/barabasi-stats", header=T, sep=",")

#data = ddply(data, c("power", "m"), function(df)c(mean(df$day)))
  
names(data) <- c("p", "k", "day")


plot(c(), xlim=c(0, 1), ylim=c(0, 250))

for(T in seq(0.01, 1, by=0.5)) {
  T_data=all_data[which(all_data[,'T'] - t < 0.001),]
  
  points(x=T_data[,'power'], y=T_data[,'day'])
}

ani.start()
for(t in seq(0.01, 1, by=0.05)) {
  data = all_data[which(all_data$T - t < 0.001),]
  fill_factor = cut.default(data$day, breaks = c(0, 30, 60, 90, 120, 150, 180, 210, 240, 270))
  colours <- rainbow_hcl(4, start = 270, end = 150)
  
  hvig_colors = c("#1e0080", "#00009a", "#0000ee", "#006577", "#33cc00", "#ddf600", "#ffbc00", "#ff6700", "#ff1200")
  
  p = ggplot(data, aes(x=power, y=k, z=day))
  p = p + geom_tile(aes(fill=fill_factor, colour = fill_factor), size=1, name="test")
  
  #p = p + scale_fill_discrete()
  
  p = p + opts(title = paste("T=", t))

  
  print(p)
}

ani.stop()