pick = function(s) {
  ix = which(colnames(sim)==s)
  sim[,ix:(ix+24)]
}

number = pick("number_0")
mass = pick("mass_0")
increment = pick("dy_0")

