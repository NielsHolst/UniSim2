
meltxy = function(df, xvars, yvars) {
	M = melt(df, measure.vars=xvars, value.name="xValue", variable.name="xVariable")
	melt(M, id.vars=c("xValue","xVariable"), measure.vars=yvars, value.name="yValue", variable.name="yVariable")
}

cols = colnames(sim)[-(1:2)]
stat = cbind(
	ddply(sim, .(iteration), colwise(min, cols))
	,
	ddply(sim, .(iteration), colwise(max, cols))
	,
	ddply(sim, .(iteration), colwise(which.max, cols))
	,
	ddply(sim, .(iteration), colwise(tail, cols), n=1)
)
keep = colnames(stat) != "iteration"
keep[1] = TRUE
stat = stat[,keep]

stat_names = c("min", "max", "max.at", "end")
ncols = length(cols)
nstat = length(stat_names)

new_names = "iteration"                                                                          
for (i in 0:(nstat*ncols-1)) {
	stat_name = stat_names[i%/%ncols + 1]
	variable_name = cols[i%%ncols + 1]
	print (paste(i%/%ncols, i%%ncols))
	name = paste()
	new_names = c(new_names, paste(variable_name, stat_name, sep="."))
}

new_names
colnames(stat) = new_names

inputs = c("duration", "initial")
inputValues = ddply(sim, .(iteration), colwise(unique, inputs))

stat = cbind(stat, inputValues[, inputs])
stat

#facetted
V = meltxy(stat, inputs, c("larva.content.max.at","adult.content.max.at"))
ggplot(V, aes(x=xValue, y=yValue)) +
	geom_point() +
	geom_smooth() +
	labs(x="", y="") +
	facet_grid(yVariable~xVariable, scales="free")

#merged
ggplot(V, aes(x=xValue, y=yValue, colour=yVariable)) +
	geom_point() +
	geom_smooth() +
	labs(x="", y="") +
	facet_wrap(~xVariable, scales="free")

	

# M = melt(stat[, c("iteration", "larva.content.max.at","adult.content.max.at")], id.vars="iteration")


# ggplot(M, aes(x=)

