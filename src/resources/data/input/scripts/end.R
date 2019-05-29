sim = read_output(output_file_name)

if (!exists("output_skip_formats")) {
  plot_all(sim)
}

if (exists("sobol_N")) {
  # library(GGally)

  colnames(sim)[input_columns()] = input_names() # Remove ".end" from input column names

  # open_plot_window(9,9)
  # M = sim[1:(2*sobol_N), input_names()]
  # print(ggpairs(M))

  n_outputs = length(output_names())
  open_plot_window(9,4)
  print( plot_against_sample_size() )
  
  print(paste0("Bootstrapping (n=", sobol_B, ")..."))
  B = adply(1:n_outputs, 1, sobol_bootstrap, n=sobol_B)
  S = ddply(B, .(Output), sobol_statistics)
  print(S)
 
  list_of_plot_effects = dlply(S, .(Output), plot_effects)
  l_ply(list_of_plot_effects, function(x) { open_plot_window(14,9); print(x) } )
  
  # open_plot_window(9,9)
  # print( grid.arrange(grobs=list_of_plot_effects) )  
}

