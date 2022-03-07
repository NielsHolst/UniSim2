plot_all(sim)

ix = which(colnames(sim)=="initHostJuvenile") - 1
final = tail(sim[,-(1:ix)],1)
print(t(round(final,3)))

with (final, print(paste("initHostJuvenile  = e+h :", initHostJuvenile, "=", e+h)))
with (final, print(paste("e = f+i : ", e, "=", f+i)))
with (final, print(paste("f = g+j : ", f, "=", g+j)))
with (final, print(paste("g = k   : ", g, "=", k)))
with (final, print(paste("initHostJuvenile+initHostAdult = p+q+r+s : ", initHostJuvenile+initHostAdult, "=", p+q+r+s)))
with (final, print(paste("hostFecundity*(initHostJuvenile+initHostAdult) = o : ", hostFecundity*(initHostJuvenile+initHostAdult), "=", o)))

with (final, print(paste("initVectorOffspring = e = g :", initVectorOffspring, "=", e_, "=", g_)))
with (final, print(paste("initVectorOffspring = m+n+o :", initVectorOffspring, "=", m_+n_+o_)))
with (final, print(paste("vectorFecundity*initVectorOffspring = p+q+r :", vectorFecundity*initVectorOffspring, "=", p_+q_+r_)))

