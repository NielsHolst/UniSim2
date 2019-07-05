sim = read_output(output_file_name)
x = subset(sim, dateTime >= dmy("3/6/2001"))
plot_all(x)
