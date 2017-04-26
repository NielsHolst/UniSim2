library(ggplot2)
library(gridExtra)
library(lubridate)
library(plyr)
library(reshape2)
library(scales)

if (!keepPlots) graphics.off ()
if (!keepVariables) rm(list=ls(all=TRUE))

read_output = function(file_path) {

	column_info = function(file_path) {
		sip = read.table(file_path, header=TRUE, sep="\t", stringsAsFactors=FALSE, nrows=1)
		Rformat = unlist(sip[1,])
		ix_date = (Rformat == "ymd") | (Rformat == "hms") | (Rformat == "ymdhms")
		read_format = Rformat
		read_format[ix_date]  = "character"
		names(read_format) = NULL
		Rformat[!ix_date]  = NA
		list(col_names=colnames(sip), Rformat=Rformat, read_format=read_format)
	}

	# Extract column info
	cl = column_info(file_path)

	# Read data frame
	U = read.table(file_path, header=FALSE, sep="\t", colClasses = cl$read_format, skip=2)
	colnames(U) = cl$col_names

	# Convert date/time columns
	ix_date = which(!is.na(cl$Rformat))
	if (length(ix_date) > 0 ) {
		for (i in 1:length(ix_date)) {
			ix = ix_date[i]
			U[,ix] = parse_date_time(U[,ix], c(cl$Rformat[ix]))
		}
	}
	U
}

convert = function(df) {
	ix_bool = which(sapply(df,class)=="logical")
	df[,ix_bool] = as.numeric(df[,ix_bool])
	df
}

melted = function(df, id_x, id_iteration, cols) {
  cols_id = c(id_x, id_iteration)
  cols_final = union(cols, cols_id)
  M = melt(convert(df)[ ,cols_final], id.vars=cols_id, value.name="Value", variable.name="Variable")
  if (!is.null(id_iteration)) M$iter = factor(M[,id_iteration])
  M
}

plot_facetted_one_x = function(df, id_x, id_iteration, cols, ncol, nrow) {
  M = melted(df, id_x, id_iteration, cols)
	hasIterations = !is.null(M$iter)
	onlyOneVariable = (length(cols) == 1)

	color = if (hasIterations & onlyOneVariable) "iter" else "Variable"
	P = ggplot(M, aes_string(x=id_x, y="Value", color=color)) +
		theme(legend.position="none") +
		xlab("") + ylab("")

  if (hasIterations)
		P + facet_grid(iter~Variable, scales="free_y") else
		P + facet_wrap(~Variable, ncol=ncol, nrow=nrow, scales="free_y") 
}

plot_merged_one_x = function(df, id_x, id_iteration, cols, ncol, nrow) {
  M = melted(df, id_x, id_iteration, cols)
	hasIterations = !is.null(M$iter)
	onlyOneVariable = (length(cols) == 1)

	color = if (hasIterations & onlyOneVariable) "iter" else "Variable"
	P = ggplot(M, aes_string(x=id_x, y="Value", color=color)) 
	if (onlyOneVariable) {
		P = P + xlab(id_x) + ylab(cols)
	} else {
		P = P + xlab("") + ylab("")
	}
	
	if (hasIterations & !onlyOneVariable) P = P + facet_wrap(~iter, ncol=ncol, nrow=nrow)
  P
}

meltxy = function(df, xvars, id_iteration, yvars) {
	M = melt(convert(df), measure.vars=xvars, value.name="xValue", variable.name="xVariable")
	M = melt(M, id.vars=c(id_iteration,"xValue","xVariable"), measure.vars=yvars, value.name="ResponseValue", variable.name="Response")
  if (!is.null(id_iteration)) M$iter = factor(M[,id_iteration])
  M
}

plot_facetted_many_x = function(df, id_x, id_iteration, cols, ncol, nrow) {
  M = meltxy(df, id_x, id_iteration, cols)
	hasIterations = !is.null(M$iter)

	color = if (hasIterations) "iter" else "Response"
	ggplot(M, aes_string(x="xValue", y="ResponseValue", color=color)) +
	  xlab("") + ylab("") +
		facet_grid(Response~xVariable, scales="free") 
}

plot_merged_many_x = function(df, id_x, id_iteration, cols, ncol, nrow) {
  M = meltxy(df, id_x, id_iteration, cols)
	hasIterations = !is.null(M$iter)
	onlyOneVariable = (length(cols) == 1)
	useIter = (hasIterations & onlyOneVariable) 

	color = if (useIter) "iter" else "Response"
	p = ggplot(M, aes_string(x="xValue", y="ResponseValue", color=color)) +
	  xlab("")  + ylab("")
	p = p + if (useIter) facet_grid(Response~xVariable, scales="free") else facet_wrap(~xVariable, scales="free") 
	p
}

plot_facetted_filtered = function(df, id_x, cols, ncol, nrow) {
  M = meltxy(df, id_x, NULL, cols)
	ggplot(M, aes(x=xValue, y=ResponseValue, color=Response)) +
	  xlab("") + ylab("") +
		theme(legend.position="none") +
		facet_grid(Response~xVariable, scales="free")
}

plot_merged_filtered = function(df, id_x, cols, ncol, nrow) {
  M = meltxy(df, id_x, NULL, cols)
	ggplot(M, aes(x=xValue, y=ResponseValue, color=Response)) +
	  xlab("") + ylab("") +
		facet_wrap(~xVariable, scales="free")
}

plot_facetted = function(df, id_x, id_iteration, cols, ncol, nrow) {
	if (id_iteration == "iteration.end")
		plot_facetted_filtered(df, id_x, cols, ncol, nrow) else
		if (length(id_x) == 1) 
			plot_facetted_one_x( df, id_x, id_iteration, cols, ncol, nrow) else
			plot_facetted_many_x(df, id_x, id_iteration, cols, ncol, nrow)
}

plot_merged = function(df, id_x, id_iteration, cols, ncol, nrow) {
	if (id_iteration == "iteration.end")
		plot_merged_filtered(df, id_x, cols, ncol, nrow) else
		if (length(id_x) == 1) 
			plot_merged_one_x( df, id_x, id_iteration, cols, ncol, nrow) else
			plot_merged_many_x(df, id_x, id_iteration, cols, ncol, nrow)
}


