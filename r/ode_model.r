library(deSolve)

setwd("/home/herb/git/genetic-odes/c/data")
data = read.csv("runs.csv")

plot(x=c(), y=c(), xlim=c(0, 100), ylim=c(0, 2))


for(i in seq(1, length(data$end_fitness))) {
  params <- c(b  = data$beta[i], g = data$gamma[i], m = data$mu[i])    # contact rate
  			
  init.values <- c(S = 0.999, I = 0.001, R=0)
  
  times <- seq(0, 100, by = 1)
  #	Now we can define the differential equation model:
  
  SI <- function(time, y.values, parameters) {  # Don't change this
    with(as.list(c(y.values, parameters)), {  # or this
      dS.dt = eval(parse(text=as.character(data$S[i])))
      dI.dt = eval(parse(text=as.character(data$I[i])))
      dR.dt = eval(parse(text=as.character(data$R[i])))
  
    	return(list(c(dS.dt, dI.dt, dR.dt)))		 
    })
  }
  
  out <- as.data.frame(ode(func = SI, y = init.values, parms = params, times = times))

  lines(out$time, out$I, type = "l", xlab = "time", ylab = "percent of population", main = "SI Model Derived", lwd = 2, col="gray")
  
  print(i)
}


params <- c(beta  = .6, gamma = .3, mu = 0.02)    # contact rate
  		
init.values <- c(S = 0.999, I = 0.001, R=0)

times <- seq(0, 100, by = 1)
times
#	Now we can define the differential equation model:

SI <- function(time, y.values, parameters) {  # Don't change this
  with(as.list(c(y.values, parameters)), {  # or this
    dS.dt = -1 * beta * S * I
    dI.dt = beta * S * I - gamma * I
    dR.dt = gamma * I

  	return(list(c(dS.dt, dI.dt, dR.dt)))		 
  })
}
  
out <- as.data.frame(ode(func = SI, y = init.values, parms = params, times = times))

lines(out$time, out$I, type = "l", xlab = "Time", ylab = "Percent of Population", main = "SIR Model", lwd = 2)
