sim = read_output(output_file_name)
# plot_all(sim)

M = subset(sim, iteration==2)
summary(M)
ix = !complete.cases(M)
head(M[ix,])


# # Write sim data frame to file
# setwd(output_data_folder)
# colnames(sim) = unique_names(colnames(sim))
# print(paste("Saving sim data frame to", getwd()))
# save(sim, file=paste0("sim-N", sobol_N, "-B", sobol_B, ".Rdata"))

