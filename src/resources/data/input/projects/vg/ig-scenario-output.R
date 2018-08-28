
rownames(sim) = NULL
sim$Number = as.numeric(rownames(sim))%/%3

means = function(M) {
  n = ncol(M)
  if (n<3) return(NULL)
  R1 = M[1, 2]
  R2 = data.frame(t(colMeans(M[,3:(n-1)])))
  cbind(dateTime=R1, R2)
}

means(sim[1:3,])

M = ddply(sim, .(Number), means)
M = M[,-1]
M

plot_all(M)

write.table(M, "ig-scenario.txt", quote=FALSE, sep="\t", row.names=FALSE)
