sim = read_output(output_file_name)
sim = subset(sim, year(dateTime)==2001)
plot_all(sim)

# from = sim$dateTime[1]
# hour(from) = 11
# minute(from) = 30
# to = from
# hour(to) = 12
# minute(to) = 0

# sim1 = subset(sim, dateTime>=from & dateTime<=to)
# plot_all(sim1)
