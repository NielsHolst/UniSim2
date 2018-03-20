# Read output
sim = read_output(output_file_name)

# Re-order iterations according to value of Kprop
key = unique(sim$iteration)
iteration_order = order(unique(sim$Kprop), decreasing=TRUE)
sim$iteration = factor( match(sim$iteration, iteration_order) )

# Show plot
p = ggplot(sim, aes(x=time, y=value, colour=iteration)) +
	geom_point() + 
	geom_line() +
	xlab("Time (h)") +
	ylab(expression(paste("Crack ventilation ", (m^3/m^3/h)))) +
	labs(color='Kprop') +
	scale_colour_hue(labels=round(unique(sim$Kprop),3)[iteration_order])
print(p)
