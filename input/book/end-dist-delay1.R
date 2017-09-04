# Replace iteration numbers with k values
sim = read_output(output_file_name)
sim$iteration = factor(sim$iteration)
k = unique(sim$k)
levels(sim$iteration) = k
plot_all(sim)

# Calculate average and variance from matrix with 2 columns (x and y)
moments = function(x,y) {
	average = sum(x*y)/sum(y)
	data.frame(Average = average, 
	           Variance = sum(y*(x - average)^2)/sum(y),
						 Sum = sum(y))
}

M = ddply(sim, .(iteration), function(x) moments(x$step, x$outflow))
M$VarianceAnalytical = 100^2/k
colnames(M)[1] = "k"
print(M)


