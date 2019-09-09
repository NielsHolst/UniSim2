library(ggplot2)
rm(list=ls(all=TRUE))
graphics.off()

obs = data.frame(
  T = c(15,17,20,25,27,30,33,35),
  L = c(Inf, 
        121.5, 
        (83.68+78.46)/2,
        (47.97+41.32)/2,
        (38.92+34.42)/2,
        (35.05+32.41)/2,
        (38.33+37)/2,
        Inf)
)
obs$DevRate = 1/obs$L

model = lm(DevRate~T, data=obs[1:5,])
icept = coef(model)[1]
slope = coef(model)[2]
T0 = -icept/slope
duration = 1/slope
T0
duration

model2 = nls(DevRate~a*T*(T-T0)*sqrt(pmax(Tm-T,0)), data=obs,
         control = nls.control(maxiter=1000),
         start=list(a=3.2e-5, T0=14, Tm=34),
         algorithm="port")
         
a = coef(model2)[1]
T02 = coef(model2)[2]
Tmax2 = coef(model2)[3]
a
T02
Tmax2


dd = function(T, T0, Topt, Tmax, duration) {
  y = pmax(T - T0, 0)
  y2 = pmax((Topt - T0)*(Tmax - T)/(Tmax - Topt), 0)
  ix = T>Topt
  y[ix] = y2[ix]
  y/duration
}

brieri = function(T, a, T0, Tmax) {
  y = rep(0, length(T))
  ix = T<Tmax
  y[ix] = pmax(a*T[ix]*(T[ix]-T0)*sqrt(Tmax-T[ix]), 0)
  y
}

diff_brieri = function(T, a, T0, Tmax) {
  y = rep(0, length(T))
  ix = T<Tmax
  y[ix] = pmax(a*(-2*T0*Tmax + 3*T0*T[ix] + 4*Tmax*T[ix] - 5*T^2)/(2*sqrt(Tmax - T[ix])), 0)
  y
}

init_slope_brieri = function(a, T0, Tmax) {
  a*T0*sqrt(Tmax-T0)
}

a_brieri = function() {
  T0 = 14.17
  Tmax = 35.76
  slope/(T0*sqrt(Tmax-T0))
}

x=seq(12,37,0.2)
DD = data.frame(
  T = x,
  DevRate = dd(x, T0, 29, 35, duration)
)

B = data.frame(
  T = x,
  DevRate = brieri(x, a, T02, Tmax2)
)
  
ggplot(obs, aes(x=T, y=DevRate)) +
  geom_abline(intercept=icept, slope=slope, colour="blue", size=3) +
  geom_line(data=DD, colour="orange", size=1) + 
  geom_line(data=B, colour="yellow", size=1) + 
  geom_point(colour="red", size=3) +
  geom_vline(xintercept=T0) +
  geom_hline(yintercept=0) +
  scale_x_continuous(breaks=c(T0,20,25,30,35), 
    labels=c(round(T0,1), 20,25,30,35)) +
  labs(x="Temperature (C)", y="Development rate (per day)")
  
windows(8,6)
ggplot(obs, aes(x=T, y=DevRate*duration)) +
  geom_line(data=DD, colour="orange", size=1) + 
  geom_vline(xintercept=T0) +
  geom_hline(yintercept=0) +
  scale_x_continuous(breaks=c(T0,20,25,30,35), 
    labels=c(round(T0,1), 20,25,30,35)) +
  labs(x="Temperature (C)", y="Time step (day-degrees per day)")

  
 Topt2 = 1/10*(sqrt(9*T02^2 - 16*T02*Tmax2 + 16*Tmax2^2) + 3*T02 + 4*Tmax2)
 Topt2
 
 print(paste(
 "Tmax = ", (Topt2*(5*Topt2 - 3*T02))/(4*Topt2 - 2*T02)
 ))
