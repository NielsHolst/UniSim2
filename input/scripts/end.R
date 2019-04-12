sim = read_output(output_file_name)

if (!exists("output_skip_formats")) {
  plot_all(sim)
}

if (exists("sobol_N")) {
  n_outputs = length(output_names())
  open_plot_window(9,4)
  print( plot_against_sample_size() )
  
  print(paste0("Bootstrapping (n=", sobol_B, ")..."))
  B = adply(1:n_outputs, 1, sobol_bootstrap, n=sobol_B)
  S = ddply(B, .(Output), sobol_statistics)
  print(S)
 
  list_of_plot_effects = dlply(S, .(Output), plot_effects)
  open_plot_window(9,9)
  print( grid.arrange(grobs=list_of_plot_effects) )
}