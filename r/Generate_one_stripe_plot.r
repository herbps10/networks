library(plyr)
library(ggplot2)
library(gtools)



#data = read.table("/home/herb/git/networks/data/stats-2.000000.csv", header=T, sep=",")
#data = read.table("/home/herb/git/networks/data/stats-0.025000.csv", header=T, sep=",")
data = read.table("/home/herb/git/networks/data/stats-by-t-50-reps.csv", header=T, sep=",")


data = ddply(data, c("p", "k"), function(df)c(mean(df$day),mean(df$times_sick), mean(df$max_infected)))
  
names(data) <- c("p", "k", "day", "times_sick", "max_infected")


my_theme <- function(base_size = 12) {
  structure(list(
    axis.line =         theme_blank(),
    axis.text.x =       theme_text(size = base_size * 0.8 , lineheight = 0.9, colour = "black", vjust = 1),
    axis.text.y =       theme_text(size = base_size * 0.8, lineheight = 0.9, colour = "black", hjust = 1),
    axis.ticks =        theme_segment(colour = "black"),
    axis.title.x =      theme_text(size = base_size * 1.8, lineheight = base_size * 2, vjust = 0.5),
    axis.title.y =      theme_text(size = base_size * 1.8, lineheight = base_size * 2, angle = 90, vjust = 0.5),
    axis.ticks.length = unit(0.15, "cm"),
    axis.ticks.margin = unit(0.1, "cm"),
 
    legend.background = theme_rect(colour="white"), 
    legend.key =        theme_rect(fill = "grey95", colour = "white"),
    legend.key.size =   unit(1.2, "lines"),
    legend.text =       theme_text(size = base_size * 0.8),
    legend.title =      theme_text(size = base_size * 0.8, face = "bold", hjust = 0),
    legend.position =   "right",
 
    panel.background =  theme_rect(fill = "grey90", colour = NA), 
    panel.border =      theme_rect(colour = "black", size=1), 
    panel.grid.major =  theme_line(colour = NA, size=0),
    panel.grid.minor =  theme_line(colour = NA, size=0),
    panel.margin =      unit(0.25, "lines"),
 
    strip.background =  theme_rect(fill = "grey80", colour = NA), 
    strip.text.x =      theme_text(size = base_size * 0.8),
    strip.text.y =      theme_text(size = base_size * 0.8, angle = -90),
 
    plot.background =   theme_rect(colour = NA, fill = "white"),
    plot.title =        theme_text(size = base_size * 1.2),
    plot.margin =       unit(c(1, 1, 0.5, 0.5), "lines")
  ), class = "options")
}

#fill_factor = quantcut(data$day, q = seq(0, 1, by=0.11111))
fill_factor = quantcut(data$day, breaks = c(0, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1900), labels=c("200", "400", "600", "800", "1000", "1200", "1400", "1600", "1800+"))
colours <- rainbow_hcl(4, start = 270, end = 150)

hvig_colors = c("#1e0080", "#00009a", "#0000ee", "#006577", "#33cc00", "#ddf600", "#ffbc00", "#ff6700", "#ff1200")
normal_colors = c()

p = ggplot(data, aes(x=p, y=k, z=day))
p = p + geom_tile(aes(fill=fill_factor, colour = fill_factor), size=1, name="test")

p = p + scale_fill_manual("", values = hvig_colors)
p = p + scale_colour_manual("", values = hvig_colors)

#p = p + opts(title = "R0=2, 5 Initial Infected, 50 Repititions, N=10,000")
p = p + scale_x_continuous(expand=c(0,0)) + scale_y_continuous(expand=c(0,0))

p = p + geom_segment(x = 0.05, y = 24, xend=0.1, yend=30, colour="white")
p = p + geom_segment(x = 0.2, y = 19, xend=0.23, yend=23, colour="white")
p = p + geom_segment(x = 0.5, y = 17, xend=0.5, yend=10, colour="white")
p = p + geom_segment(x = 0.8, y = 17, xend=0.8, yend=10, colour="white")

p = p + geom_point(aes(x = c(0.05, 0.2, 0.5, 0.8), y = c(24, 19, 17, 17)))

p = p + annotate("text", x=0.105, y=30.9, label="A", colour="white")
p = p + annotate("text", x=0.235, y=23.9, label="B", colour="white")
p = p + annotate("text", x=0.5, y=9.1, label="C", colour="white")
p = p + annotate("text", x=0.8, y=9.1, label="D", colour="white")

p = p + geom_abline(intercept = 26, slope=0, colour = "white")
p = p + geom_abline(intercept = 13, slope=0, colour = "white")
p = p + geom_text(aes(x = 0.2, y=14, label="R0=1"), colour="white", size=4)
p = p + geom_text(aes(x = 0.2, y=27, label="R0=2"), colour="white", size=4)

p = p + my_theme()

print(p)


#p = p + scale_fill_continuous("day", low="white", high="black", breaks = seq(0, 1500, 300))
#p = p + scale_colour_continuous("day", low="white", high="black", breaks = seq(0, 1500, 300))
#p = p + scale_fill_brewer("", 2)
#p = p + scale_colour_brewer("", 2)

#p = p + scale_fill_hue()
#p = p + scale_colour_hue()



ggsave(p, file="/home/herb/git/networks/disease-persistence/persistence-labeled-points-with-dots-and-r0-2.pdf")


print(p)

p = p + geom_tile(aes(colour = day), fill=NA)


#p = p + geom_abline(intercept=14, slope = 0, colour="black")
#p = p + geom_abline(intercept=27, slope=0, colour="black")


#p = p + geom_text(aes(x = 0.2, y=13, label="R0=1"), colour="black", size=10)
#p = p + geom_text(aes(x = 0.2, y=26, label="R0=2"), colour="black", size=10)

#p = p + opts(title = "T=0.025, 1 Initial Infected, 1 Repitition")

print(p)
      