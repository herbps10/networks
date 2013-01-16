data = read.table("/home/herb/git/networks/data/one.csv", header=T, sep=",")

plot(data$susceptible, type="l", ylim=c(0, 10000), xlim=c(0, 10000), col="black", xlab="Day", ylab="Individuals", main="SEIR Plot")
lines(data$latent, col="gray")
lines(data$infectious, col="red")
lines(data$recovered, col="green")
legend(x="right", legend=c("Susceptible", "Latent", "Infectious", "Recovered"), fill=c("black", "gray", "red", "green"))


plot(data$infectious, type="l", xlim=c(0, 10000), col="black", xlab="Day", ylab="Number of Infected Hosts", ylim=c(0, 1000))

abline(v=3500)
abline(v=4500)


b = spectrum(data$infectious)                # conduct the spectral decomposition
plot(b$freq[1:100],b$spec[1:100], type="h", ylab="", xlab="", cex.axis=1)

plot(data$infectious[2000:3000], type="l", ylab="", xlab="")

spec.pgram(data$infectious)

infected = ts(data$infectious,frequency=365)
recovered = ts(data$recovered, frequency=365)

d = decompose(i)

seasonally_adjusted = i - d$seasonal

n <- length(x)
sort(x,partial=n-1)[n-1]


a = as.vector(acf(d$seasonal, lag.max=1865)$acf)

peaks<-function(series,span=3)
{
z <- embed(series, span)
s <- span%/%2
v<- max.col(z) == 1 + s
result <- c(rep(FALSE,s),v)
result <- result[1:(length(result)-s)]
result
}

seq(1, 1865)[peaks(a)]