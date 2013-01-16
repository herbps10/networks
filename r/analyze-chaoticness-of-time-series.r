library(plyr)
library(ggplot2)
library(gtools)

setwd("/home/herb/git/networks/data/")

snr_data = data.frame(p=character(), k=character(), snr=character(), rep=character())


for(k in seq(10, 39)) {
  for(p in seq(0.01, 0.99, by=0.01)) {
    print(paste(k, p))
    for(rep in seq(0)) {
      prefix = "many-20-reps/"
      if(rep > 19) {
        prefix = "many/"
        rep = rep - 20
      }
      
      filename = paste(prefix, "stats-", k, "-", p, "-", rep, ".csv", sep='')
      
      if(p == 0)
      {
        filename =  paste(prefix, "stats-", k, "-0.00-", rep, ".csv", sep='')
      }
      else if ((p * 100) %% 10 == 0 )
      {
        filename =  paste(prefix, "stats-", k, "-", p, "0-", rep, ".csv", sep='')
      }
  
      dat = read.csv(filename)
      
      #plot(dat$infectious, type="l")
      
      m = max(dat$infectious)
      snr_data = rbind(snr_data, c(k, p, m, rep))
      
      if(length(dat$infectious) <= 10)
      {
        #m = -1
        #snr_data = rbind(snr_data, c(k, p, m, rep))
      }
      else
      {
        #b = spectrum(dat$infectious)
        #plot(b$freq,b$spec,type = "h") # plot spectrum
        
        #DF = 1/b$freq[which(b$spec== max(b$spec))] # get dominant frequency in months
        
        #print(DF)
        
        # Average of the noise
        # take the total average and subtract the maximum value (the signal)
        
        #mean(b$spec) - max(b$spec) / length(b$spec)
        
        #snr = (max(b$spec) / (mean(b$spec) - max(b$spec) / length(b$spec)))^2
        
        #standard_deviation = sd(b$freq)
        #m = b$freq[which(b$spec == max(b$spec))]
        
        #knots = ceiling(length(dat) / DF)
        #if(knots <= 2) knots = length(dat)
        
        #spline = smooth.spline(dat$infectious, all.knots = FALSE, nknots = knots)
        #spline = smooth.spline(dat$infectious)
        #m = sum((dat$infectious - spline$y)^2) / length(dat$infectious)
        
        #snr_data = rbind(snr_data, c(k, p, m, rep))
      }
    }
  }
}

names(snr_data) <- c("k", "p", "snr", "rep")

snr_data = ddply(snr_data, c("p", "k"), function(df)c(mean(df$snr)))
    
names(snr_data) <- c("p", "k", "snr")
    
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
fill_factor = #quantcut(snr_data$snr, q = seq(0, 1, by=1/9))
fill_factor = cut(snr_data$snr, breaks=9)
#fill_factor = 
#colours <- rainbow_hcl(4, start = 270, end = 150)

hvig_colors = c("#1e0080", "#00009a", "#0000ee", "#006577", "#33cc00", "#ddf600", "#ffbc00", "#ff6700", "#ff1200")
normal_colors = c()

p = ggplot(snr_data, aes(x=p, y=k, z=snr))
p = p + geom_tile(aes(fill=fill_factor, colour = fill_factor), size=1, name="test")

#p = p + scale_fill_continuous("day", low="white", high="black")
#p = p + scale_colour_continuous("day", low="white", high="black")
#p = p + scale_fill_brewer("", 2)
#p = p + scale_colour_brewer("", 2)

#p = p + scale_fill_hue()
#p = p + scale_colour_hue()

p = p + scale_fill_manual("", values = hvig_colors)
p = p + scale_colour_manual("", values = hvig_colors)

p = p + opts(title = "Signal to Noise Ratio, 50 Repititions, T=0.025")
p = p + scale_x_continuous(expand=c(0,0)) + scale_y_continuous(expand=c(0,0))
p = p + my_theme()

#p = p + geom_abline(intercept = 26, slope=0, colour = "white")
#p = p + geom_abline(intercept = 13, slope=0, colour = "white")

#p = p + geom_text(aes(x = 0.2, y=14, label="R0=1"), colour="white", size=4)
#p = p + geom_text(aes(x = 0.2, y=27, label="R0=2"), colour="white", size=4)


#ggsave(p, file="/home/herb/git/networks/disease-persistence/snr-50-repititions-t-0.025.png")

print(p)