# Replace iteration numbers with location names
sim = read_output(output_file_name)
sim$iteration = factor(sim$iteration)
levels(sim$iteration) = c("Brazil_RCP45", "Brazil_RCP85" , "CostaRica_RCP45", "CostaRica_RCP85")
  
plot_all(sim)