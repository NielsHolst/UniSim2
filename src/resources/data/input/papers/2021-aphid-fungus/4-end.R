source(paste0(box_script_folder,"/configure.R"))

sim = read_output(output_file_name)
plot_all(sim)

# Write sim data frame to file
setwd(output_data_folder)
print(paste("Saving sim data frame to", getwd()))
save(sim, file=paste0("sim-N", sobol_N, "-B", sobol_B, ".Rdata"))

# colnames(sim) = unique_names(colnames(sim))

# ggarrange(
  # ggplot(sim, aes(maxPrevalenceGS)) +
    # geom_histogram(colour=red, fill=red, alpha=0.3) + scale_y_log10() +
    # theme_classic()
  # ,
  # ggplot(sim, aes(maxCadaverPrevalenceGS)) +
    # geom_histogram(colour=violet, fill=violet, alpha=0.3) + scale_y_log10() +
    # theme_classic()
  # ,
  # ncol=1, align="hv"
# )

# nrow(subset(sim, maxPrevalenceGS>55 & maxPrevalenceGS<60))
# nrow(subset(sim, maxCadaverPrevalenceGS>55 & maxCadaverPrevalenceGS<60))


