# Pmax - g
p1 = c(0.012, 0.00078)
p2 = c(0.015, 0.00094)
slope = (p2[2]-p1[2])/(p2[1]-p1[1])
icept = p1[2]-slope*p1[1]
print(c(slope=slope, intercept=icept))

# beta - Pmax
p1 = c(0.00100, 0.00390)
p2 = c(0.00106, 0.00415)
slope = (p2[2]-p1[2])/(p2[1]-p1[1])
icept = p1[2]-slope*p1[1]
print(c(slope=slope, intercept=icept))

# plot_g_beta
p1 = c(0.012, 0.0034)
p2 = c(0.014, 0.0041)
slope = (p2[2]-p1[2])/(p2[1]-p1[1])
icept = p1[2]-slope*p1[1]
print(c(slope=slope, intercept=icept))

p1 = c(0.012, 0.0020)
p2 = c(0.014, 0.0025)
slope = (p2[2]-p1[2])/(p2[1]-p1[1])
icept = p1[2]-slope*p1[1]
print(c(slope=slope, intercept=icept))

# plot uCinit-Pmax
p1 = c(0.0012, 0.95)
p2 = c(0.0014, 0.60)
slope = (p2[2]-p1[2])/(p2[1]-p1[1])
icept = p1[2]-slope*p1[1]
print(c(slope=slope, intercept=icept))

# plot Pmax-g
p1 = c(0.0050, 0.00091)
p2 = c(0.0055, 0.00105)
slope = (p2[2]-p1[2])/(p2[1]-p1[1])
icept = p1[2]-slope*p1[1]
print(c(slope=slope, intercept=icept))


