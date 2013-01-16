library(ggplot2)

data = data.frame()

T = 0.025
d = 3
R0 = 2

R0s = data.frame() # Holds the K values at which R0 is equal to one

for(T in seq(0, 1, by=0.005)) {
  data = data.frame()
  for(k in seq(2, 60)) {
    R0_calc = k - k * (1-T) ^ d
    T_calc = 1 - (1 - (R0/k)) ^ (1/d)
      
    data = rbind(data, c(k, R0_calc, T_calc))
  }
  
  names(data) <- c("k", "R0", "T")
  
  intercept = coefficients(lm(data$R0~data$k))[[1]]
  slope = coefficients(lm(data$R0~data$k))[[2]]
  
  R0_zero = (1 - intercept) / slope

  R0s = rbind(R0s, c(T, R0_zero))
}

names(R0s) <- c("T", "k")
plot(R0s$T, R0s$k, type="l", ylab="k at which R0=1", xlab="T", main="Epidemic Threshold")
abline(h=1, lty=3)
text(0, 2, label="k = 1")


par(mfrow=c(1,1))
plot(data$k, data$T, xlab="K", ylab="T", main="T vs K")

plot(data$k, data$R0, xlab="K", ylab="R0", main="Theoretical R0 versus K, T = 0.02", ylim=c(0, 6), xlim=c(0, 60))
abline(h=1, lty=3)

lm(data$R0~data$k)

for(r in seq(1, 4)) {
  k = (r - 9.250e-16) / 7.4314e-2
  print(k)
}