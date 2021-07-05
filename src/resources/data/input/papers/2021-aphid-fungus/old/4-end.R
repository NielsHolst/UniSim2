source(paste0(box_script_folder,"/configure.R"))

sim = read_output(output_file_name)
plot_all(sim)

# Write sim data frame to file
setwd(output_file_folder())
print(paste("Saving sim data frame to", getwd()))
save(sim, file=paste0("sim-N", sobol_N, "-B", sobol_B, ".Rdata"))

