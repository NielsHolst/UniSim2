sim = read_output(output_file_name)
days = sim[24*92+7*(0:24),]

plot_all(days)
plot_all(sim)
