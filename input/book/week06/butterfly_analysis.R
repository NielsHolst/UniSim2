sim = read_output(output_file_name)
sim$iteration = factor(sim$iteration)
levels(sim$iteration) = c("Flakkebjerg", "Braunschweig", "Geneva", "Ljubljana")
plot_all(sim)

sim$Location = sim$iteration
sim$day.num = yday(sim$date)

to_date = function(x) {
	dmy("31/12/2008") + days(round(x))
}

stats = function(df) {
	f = approxfun(df$adult.outflowTotal, df$day.num)
	data.frame(From = to_date(f(10)), 
             Median = to_date(f(50)), 
             To = to_date(f(90)), 
             Duration = f(90) - f(10))
}

A = ddply(sim, .(Location), stats)

P = ggplot(A) +
  geom_point(aes(y=Location, x=Median, colour=Location), size=3) +
  geom_errorbarh(aes(y=Location, x=Median, xmin=From, xmax=To, 
                     height=0, colour=Location), size=1.3) +
  xlab("Butterflies flying") + ylab("")

print(P)
  