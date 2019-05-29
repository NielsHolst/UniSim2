sim = read_output(output_file_name)

n = nrow(sim)
breaks = seq(20,40,by=2)
logbreaks = 10^seq(2,4,length.out=11)

inferNormal = function(xmin, xmax, Prange) {
  Pmax = (1+Prange)/2
  xsd = qnorm(Pmax)
  data.frame(
    m = (xmax + xmin)/2,
    s = (xmax - xmin)/2/xsd
  )
}
inferNormal(-1.96, 1.96, 0.95)

P_range = 0.95

x = breaks
normal = inferNormal(20, 40, P_range)
sum_curve = pnorm(x, normal$m, normal$s)
sum_curve_scaled = (sum_curve-(1-P_range)/2)/0.95
M = data.frame(X=x, SumCurve=sum_curve, SumCurveScaled=sum_curve_scaled*n)
normal_curve = data.frame(Xmid = x[1:(length(x)-1)] + (x[2]-x[1])/2, Y=diff(M$SumCurveScaled))

x = log10(logbreaks)
lognormal = inferNormal(2, 4, P_range)
sum_curve = pnorm(x, lognormal$m, lognormal$s)
sum_curve_scaled = (sum_curve-(1-P_range)/2)/0.95
M = data.frame(X=x, SumCurve=sum_curve, SumCurveScaled=sum_curve_scaled*n)
lognormal_curve = data.frame(Xmid = 10^( x[1:(length(x)-1)] + (x[2]-x[1])/2 ), Y=diff(M$SumCurveScaled))
lognormal_curve



plots = list(
ggplot(sim) +
  geom_histogram(aes(x=uniform.value), fill="green", alpha=0.2, colour="brown", breaks=breaks, closed="left") +
  geom_hline(yintercept=n/(length(breaks)-1), colour="forestgreen", size=1) +
  labs(x="Uniform values", y="Count")
,
ggplot(sim) +
  geom_bar(aes(x=uniformInt.value), fill="brown", width=0.3) +
  geom_hline(yintercept=n/20, colour="forestgreen", size=1) +
  labs(x="UniformInt values", y="Count")
,
ggplot(sim) +
  geom_histogram(aes(x=normal.value), fill="green", alpha=0.2, colour="brown", breaks=breaks, closed="left") +
  labs(x="Normal values", y="Count") +
  geom_line(data=normal_curve, aes(x=Xmid, y=Y), colour="forestgreen", size=1) +
  geom_point(data=normal_curve, aes(x=Xmid, y=Y), colour="forestgreen", size=2) 
,
ggplot(sim) +
  geom_histogram(aes(x=lognormal.value), fill="green", alpha=0.2, colour="brown", breaks=logbreaks, closed="left") +
  scale_x_continuous(breaks = c(100, 1000, 10000))+ 
  labs(x="Lognormal values", y="Count") +
  geom_line(data=lognormal_curve, aes(x=Xmid, y=Y), colour="forestgreen", size=1) +
  geom_point(data=lognormal_curve, aes(x=Xmid, y=Y), colour="forestgreen", size=2) 
,
ggplot(sim) +
  geom_bar(aes(x=binomial.value), fill="brown", width=0.3) +
  geom_hline(yintercept=c(0.2*n, 0.8*n), colour="forestgreen", size=1) +
  labs(x="Binomial values", y="Count")
)

windows()
print( grid.arrange(grobs=plots) )

