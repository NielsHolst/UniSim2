library(reshape2)
sim = read_output(output_file_name)
sim = subset(sim, iteration!=0)

unfactor = function(x) as.numeric(as.character(x))

duration = ddply(sim, .(iteration), nrow)
duration

colnames(duration) = c("Initial", "Duration")
duration$Initial = unfactor(duration$Initial)

p = ggplot(duration, aes(x=Initial, y=Duration)) +
	geom_line(size=1, colour="blue") +
	geom_point(size=4, colour="red")

print(p)



	