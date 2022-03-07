plot_all(sim)

library(stringr)
columns = str_detect(colnames(sim), "outflowTotal")
totals = tail(sim[,columns],1)
print(totals)
print(sum(totals))
