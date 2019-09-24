sim = read_output(output_file_name)

if (!exists("output_skip_formats")) {
  if (exists("sobol_N"))
    plot_all(sim[1:sobol_N,])
  else
    plot_all(sim)
}
 

