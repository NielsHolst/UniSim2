sim = read_output(output_file_name)

if (!exists("output_skip_formats")) {
  if (exists("sobol_N"))
    plot_all(sim[1:sobol_N,])
  else
    plot_all(sim)
}

if (exists("sobol_N")) {
  colnames(sim) = unique_col_names(colnames(sim))

  n_outputs = length(output_names())
  open_plot_window(9,4)
  print( plot_against_sample_size() )
  
  print(paste0("Bootstrapping (n=", sobol_B, ")..."))
  B = adply(1:n_outputs, 1, sobol_bootstrap, n=sobol_B)
  S = ddply(B, .(Output), sobol_statistics)
  print(S)
 
  list_of_plot_effects = dlply(S, .(Output), plot_effects)
  list_of_plot_effects_no_sum = dlply(subset(S, Input!="Sum"), .(Output), plot_effects)

  l_ply(list_of_plot_effects_no_sum, function(x) { open_plot_window(14,9); print(x) } )
  
  setwd(box_script_folder)

  pdf("sobol-indices.pdf", paper="a4")
  l_ply(list_of_plot_effects, print )
  dev.off()

  pdf("sobol-indices-no-sum.pdf", paper="a4")
  l_ply(list_of_plot_effects_no_sum, print )
  dev.off()
}

 

