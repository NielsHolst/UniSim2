# Replace iteration numbers with location names
sim = read_output(output_file_name)
sim$iteration = factor(sim$iteration)
levels(sim$iteration) = c(1,2,3,10,25)
plot_all(sim)