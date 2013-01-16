data = read.table("/home/herb/git/networks/data/infection_events.csv", header=T)

plot(data$time, data$vertex, type="p", xlim=c(0, 150))

plot(seq(1, length(data$vertex)), data$vertex, type="p")