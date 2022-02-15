# Extract settings
clear_plots     = exists("clear_plots")     && clear_plots
show_plots      = exists("show_plots")      && show_plots && exists("plot_all")
skip_formats    = exists("skip_formats")    && skip_formats
save_data_frame = exists("save_data_frame") && save_data_frame

# Read output into data frame
sim = read_output(output_file_name)

# Show plots
if (clear_plots) graphics.off()
if (show_plots)  plot_all(sim)

# Save output as data frame
if (save_data_frame) {
  file_name_R = paste0(output_file_folder(), "/", output_file_base_name(), ".Rdata")
  print(paste("Writing sim data frame to", file_name_R))
  save(sim, file=file_name_R)
}
