sim = read_output(output_file_name)
plot_all(sim)

islands = c("NewHaven", "Exotica", "Lampedusa")

compare = function(df) {
	rabbit_initial = df[1, paste0(islands,".rabbit.rabbit.initial")]
	rabbit_total = tail(df[paste0(islands,".rabbit.rabbit.outflowTotal")], 1)
	R = data.frame(rabbit_initial, rabbit_total, sum(rabbit_initial), sum(rabbit_total))
	colnames(R) = c(paste0("rabbit_initial",0:2), paste0("rabbit_outflowTotal",0:2), "rabbit_initial_sum", "rabbit_outflowTotal_sum")

	fox_initial = df[1, paste0(islands, ".fox.fox.initial")]
	fox_total = tail(df[paste0(islands,".fox.fox.outflowTotal")], 1)
	F = data.frame(fox_initial, fox_total, sum(fox_initial), sum(fox_total))
	colnames(F) = c(paste0("fox_initial",0:2), paste0("fox_outflowTotal",0:2), "fox_initial_sum", "fox_outflowTotal_sum")
	
	D = data.frame(sum(rabbit_total)-sum(rabbit_initial), sum(fox_total)-sum(fox_initial))
	colnames(D) = c("rabbit_diff", "fox_diff")
	
	cbind(R,F,D)
}

A = ddply(sim, .(iteration), compare)
print(A)


	