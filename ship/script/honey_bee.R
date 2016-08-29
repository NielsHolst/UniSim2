pick_rows = function(name) {
  ix = which(colnames(sim)==name)
  sim[,ix:(ix+24)]
}

pick_col = function(ix) {
  M = data.frame(t(number[ix,]), t(mass[ix,]), t(x[ix,]), t(dx[ix,]), t(dy[ix,]))
  colnames(M) = c("number", "mass", "x", "dx", "dy")
  M
}


number = pick_rows("number_0")
mass = pick_rows("mass_0")
x = pick_rows("x_0")
dx = pick_rows("dx_0")
dy = pick_rows("dy_0")

tail(number)
tail(mass)
tail(x)
tail(dx)
tail(dy)

pick_col(31)

