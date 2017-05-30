sim = read_output(output_file_name)

M = ddply(sim[colnames(sim)!="step"], .(iteration), unique)

n = max(M$iteration)

uniform_binwidth = 2
loguniform_binwidth = 0.2
normal_bindwidth = 4

x_norm = 70 + 0:60
x_lognorm = seq(25, 400, length.out=61)

E = data.frame(
	uniform_x=(0:60)/2, uniform_y=uniform_binwidth*n/30,
	loguniform_x=(-60:0)/20, loguniform_y=loguniform_binwidth*n/3,
	normal_x=x_norm, normal_y=dnorm(x_norm, 100, 10)*normal_bindwidth*n,	
	lognormal_x=x_lognorm, lognormal_y=dnorm(log(x_lognorm), log(100), 0.5)*20 #hack scale to fit
)

P = grid.arrange(
	ggplot(M, aes(x=uniform)) +
		geom_histogram(breaks=seq(0, 30, by = uniform_binwidth), 
									 col="forestgreen", size=1, fill="green", alpha = .2) +
		geom_line(data=E, aes(uniform_x, uniform_y), color="red", size=1.1, alpha=0.6)
	,
	ggplot(M, aes(x=normal)) +
		geom_histogram(breaks=seq(70, 130, by = normal_bindwidth), 
									 col="forestgreen", size=1, fill="green", alpha = .2) +
		geom_line(data=E, aes(normal_x, normal_y), color="red", size=1.1, alpha=0.6)
	, 
	ggplot(M, aes(x=log10(loguniform))) +
		geom_histogram(breaks = seq(-3, 0, by = loguniform_binwidth),	
									 col="forestgreen", size=1, fill="green", alpha = .2) +
		geom_line(data=E, aes(loguniform_x, loguniform_y), color="red", size=1.1, alpha=0.6)
	,
	ggplot(M, aes(x=log10(lognormal))) +
		geom_histogram(breaks=seq(1.4, 2.6, length.out=15), 
									 col="forestgreen", size=1, fill="green", alpha = .2) +
		geom_line(data=E, aes(log10(lognormal_x), lognormal_y), color="red", size=1.1, alpha=0.6)
	,
	nrow = 2
)
P
# print(P)
print(paste( "Normal mean and sd =", 
              round(mean(sim$normal),3), 
							round(sd(sim$normal),3) 
))
print(paste( "Log-normal mean(log(x)) and sd(log(x)) =", 
              round(mean(log(sim$lognormal)),3), 
							round(sd(log(sim$lognormal)),3) 
))



