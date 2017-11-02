
extract = function(M) {
  n = 200
  density = tail(M$density,n)
  q = tail(M$q,n)
  data.frame(Density=median(density), q=median(q), DensitySd=sd(log(density)), qSd=sd(q))
}

E = ddply(sim, .(iteration), extract)
E

