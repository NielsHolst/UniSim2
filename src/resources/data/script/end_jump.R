sim = read_output(output_file_name)

p = ggplot(sim, aes(x=step, y=value, colour=iteration)) +
	geom_line(size=1) +
	geom_point(size=4) +
	facet_wrap(~iteration)

print(p)




	