sim = read_output(output_file_name)
sim = subset(sim, iteration!=0)
n = length(levels(sim$iteration))
# Re-order iteration
sim$iteration = factor(sim$iteration, 1:n)

p = ggplot(sim, aes(x=step, y=value, colour=iteration)) +
	geom_line(size=1) +
	geom_point(size=4) +
	facet_wrap(~iteration)

print(p)




	