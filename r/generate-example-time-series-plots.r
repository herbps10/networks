setwd("/home/herb/git/networks/disease-persistence/")

par(mfrow=c(4, 2))



data1 = read.csv("one-p-0.05-k-18.csv")
plot(data1$infectious, type="l", col='black', ylim=c(0, 1000), xlim=c(0, 10000), main="p=0.05 k=18", xlab="Timestep", ylab="Number Infected")

data1 = read.csv("one-p-0.075-k-20.csv")
plot(data1$infectious, type="l", col='black', ylim=c(0, 1000), xlim=c(0, 10000), main="p=0.075 k=20", xlab="Timestep", ylab="Number Infected")

data1 = read.csv("one-p-0.1-k-21.csv")
plot(data1$infectious, type="l", col='black', ylim=c(0, 1000), xlim=c(0, 10000), main="p=0.1 k=21", xlab="Timestep", ylab="Number Infected")

data1 = read.csv("one-p-0.125-k-22.csv")
plot(data1$infectious, type="l", col='black', ylim=c(0, 1000), xlim=c(0, 10000), main="p=0.125 k=22", xlab="Timestep", ylab="Number Infected")

data1 = read.csv("one-p-0.15-k-24.csv")
plot(data1$infectious, type="l", col='black', ylim=c(0, 1000), xlim=c(0, 10000), main="p=0.15 k=24", xlab="Timestep", ylab="Number Infected")




data1 = read.csv("one-p-0.05-k-24.csv")
plot(data1$infectious, type="l", col='black', ylim=c(0, 1000), xlim=c(0, 10000), main="1", xlab="Timestep", ylab="Number Infected")
#lines(smooth.spline(data1$infectious), col="red")

b = spectrum(data1$infectious, plot=F)
plot(b$freq[1:150],b$spec[1:150], type = "h", xlab="Frequency", ylab="Spectral Density") # plot spectrum

data2 = read.csv("one-p-0.2-k-19.csv")
plot(data2$infectious, type="l", col='black', ylim=c(0, 1000), xlim=c(0, 10000), main="2", xlab="Timestep", ylab="Number Infected")
#lines(smooth.spline(data2$infectious), col="red")

b = spectrum(data2$infectious, plot=F)
plot(b$freq[1:150],b$spec[1:150], type = "h", xlab="Frequency", ylab="Spectral Density") # plot spectrum

data3 = read.csv("one-p-0.5-k-17.csv")
plot(data3$infectious, type="l", col='black', ylim=c(0, 1000), xlim=c(0, 10000), main="3", xlab="Timestep", ylab="Number Infected")
#lines(smooth.spline(data3$infectious), col="red")

b = spectrum(data3$infectious, plot=F)
plot(b$freq[1:150],b$spec[1:150], type = "h", xlab="Frequency", ylab="Spectral Density") # plot spectrum
     
data4 = read.csv("one-p-0.8-k-17.csv")
plot(data4$infectious, type="l", col='black', ylim=c(0, 1000), xlim=c(0, 10000), main="4", xlab="Timestep", ylab="Number Infected")
#lines(smooth.spline(data4$infectious), col="red")

b = spectrum(data4$infectious, plot=F)
plot(b$freq[1:150],b$spec[1:150], type = "h", xlab="Frequency", ylab="Spectral Density") # plot spectrum

par(mfrow = c(1, 1))


par(mfrow=c(4, 2))

data1 = read.csv("one-p-0.05-k-24.csv")
plot(data1$infectious[seq(1000, length(data1$infectious))], type="l", col='black', ylim=c(0, 1000), xlim=c(1000, 10000), main="1", xlab="Timestep", ylab="Number Infected")
#lines(smooth.spline(data1$infectious), col="red")

b = spectrum(data1$infectious[seq(1000, length(data1$infectious))], plot=F)
plot(b$freq[1:150],b$spec[1:150], type = "h", xlab="Frequency", ylab="Spectral Density") # plot spectrum

data2 = read.csv("one-p-0.2-k-19.csv")
plot(data2$infectious[seq(1000, length(data2$infectious))], type="l", col='black', ylim=c(0, 1000), xlim=c(1000, 10000), main="2", xlab="Timestep", ylab="Number Infected")
#lines(smooth.spline(data2$infectious), col="red")

b = spectrum(data2$infectious[seq(1000, length(data2$infectious))], plot=F)
plot(b$freq[1:150],b$spec[1:150], type = "h", xlab="Frequency", ylab="Spectral Density") # plot spectrum

data3 = read.csv("one-p-0.5-k-17.csv")
plot(data3$infectious[seq(1000, length(data3$infectious))], type="l", col='black', ylim=c(0, 1000), xlim=c(1000, 10000), main="3", xlab="Timestep", ylab="Number Infected")
#lines(smooth.spline(data3$infectious), col="red")

b = spectrum(data3$infectious[seq(1000, length(data3$infectious))], plot=F)
plot(b$freq[1:150],b$spec[1:150], type = "h", xlab="Frequency", ylab="Spectral Density") # plot spectrum

data4 = read.csv("one-p-0.8-k-17.csv")
plot(data4$infectious[seq(1000, length(data4$infectious))], type="l", col='black', ylim=c(0, 1000), xlim=c(1000, 10000), main="4", xlab="Timestep", ylab="Number Infected")
#lines(smooth.spline(data4$infectious), col="red")

b = spectrum(data4$infectious[seq(1000, length(data4$infectious))], plot=F)
plot(b$freq[1:150],b$spec[1:150], type = "h", xlab="Frequency", ylab="Spectral Density") # plot spectrum

par(mfrow = c(1, 1))