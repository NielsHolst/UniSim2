sim = read_output(output_file_name)
plot_all(sim)

library(stringr)
columns = str_detect(colnames(sim), "outflowTotal")
totals = tail(sim[,columns],1)
print(totals)
print(sum(totals))
