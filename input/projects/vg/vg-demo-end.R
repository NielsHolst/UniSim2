sim = read_output(output_file_name)
sim = subset(sim, year(dateTime)==2001)
plot_all(sim)

# rownames(sim) = {}
# sim$i = as.numeric(rownames(sim)) - 1
# sim$hour = sim$i %/% 30

# average = function(M) {
  # # M = subset(sim, hour==10)
  # M1 = data.frame(dateTime = M[16,2])
  # M2 = t(colMeans(M[,-(1:2)]))
  # M3 = cbind(M1, M2)
  # n = ncol(M3)
  # M3[,-c(n-1,n)]
# }

# sim2 = ddply(sim, .(hour), average)

# write.table(sim,  "vg-1yr-2min.txt", quote=FALSE, sep="\t", row.names=FALSE)
# write.table(sim2, "vg-1yr-1hr.txt", quote=FALSE, sep="\t", row.names=FALSE)


# from = sim$dateTime[1]
# hour(from) = 11
# minute(from) = 30
# to = from
# hour(to) = 12
# minute(to) = 0

# sim1 = subset(sim, dateTime>=from & dateTime<=to)
# plot_all(sim1)
