# Save the sim data frame to the output folder
sim = read_output(output_file_name)
colnames(sim) = unique_names(colnames(sim))
sim = sim[,-1]
save(sim, file=paste0(output_file_folder(), "/sim.Rdata"))
