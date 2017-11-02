# Replace iteration numbers with predator density
sim = read_output(output_file_name)
sim$iteration = factor(sim$iteration)
levels(sim$iteration) = 
  c("Predator density = 0.5",
    "Predator density = 1",
    "Predator density = 20")
plot_all(sim)