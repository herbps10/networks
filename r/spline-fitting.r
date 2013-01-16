setwd("/home/herb/git/networks/disease-persistence/")
par(mfrow=c(1,1))

data = read.csv("one-p-0.05-k-24.csv")
plot(data$infectious, type="l", col='red', ylim=c(0, 10000), xlim=c(0, 10000), main="", xlab="Timestep", ylab="Individuals")
lines(data$susceptible, col="black")
lines(data$recovered, col="green")

legend("topright",  c("Susceptible", "Infectious", "Recovered"), fill=c("Black", "Red", "Green"))

b = spectrum(data$infectious)
plot(b$freq,b$spec,type = "h") # plot spectrum
      
DF = 1/b$freq[which(b$spec== max(b$spec))] # get dominant frequency in months
 

spline = smooth.spline(data$infectious, all.knots = FALSE, nknots = 10000/DF)



plot(data$infectious, type="l")
lines(spline, col="red")


attach(cars)
plot(speed, dist, main = "data(cars)  &  smoothing splines")
cars.spl <- smooth.spline(speed, dist)
(cars.spl)
## This example has duplicate points, so avoid cv=TRUE

lines(cars.spl, col = "blue")
lines(smooth.spline(speed, dist, df=10), lty=2, col = "red")
legend(5,120,c(paste("default [C.V.] => df =",round(cars.spl$df,1)),
               "s( * , df = 10)"), col = c("blue","red"), lty = 1:2,
       bg='bisque')
detach()