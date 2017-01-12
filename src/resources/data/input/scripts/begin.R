library(ggplot2)
library(gridExtra)
library(lubridate)
library(plyr)
library(reshape2)
library(scales)

rm(list=ls(all=TRUE))
graphics.off ()

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

melted = function(df, id_x, id_iteration, cols) {
  cols_id = c(id_x, id_iteration)
  cols_final = union(cols, cols_id)
  M = melt(df[ ,cols_final], id.vars=cols_id, value.name="Value", variable.name="Variable")
  if (!is.null(id_iteration)) M$iter = factor(M[,id_iteration])
  M
}

convert = function(df) {
	ix_bool = which(sapply(df,class)=="logical")
	for (i in ix_bool) {
		df[,i] = as.numeric(df[,i])
	}
	df
}

plot_facetted = function(df, id_x, id_iteration, cols, ncol, nrow) {
  M = melted(convert(df), id_x, id_iteration, cols)
	P = ggplot(M, aes_string(x=id_x, y="Value", color="Variable")) +
		geom_line(size=1.1) +
		theme(legend.position="none") +
		xlab("") + ylab("")

  if (is.null(M$iter))
		P + facet_wrap(~Variable, ncol=ncol, nrow=nrow, scales="free_y") else
		P + facet_grid(iter~Variable, scales="free_y") 
}

plot_merged = function(df, id_x, id_iteration, cols, ncol, nrow) {
  M = melted(convert(df), id_x, id_iteration, cols)
  if (length(cols) == 1 & !is.null(M$iter)) { # If only one y merge iterations
    ggplot(M, aes_string(x=id_x, y="Value", color="iter")) +
      geom_line(size=1.1) +
      xlab("") + ylab("")
  } else {
    P = ggplot(M, aes_string(x=id_x, y="Value", color="Variable")) +
      geom_line(size=1.1) +
      xlab("") + ylab("")
    if (is.null(M$iter))
      P else
      P + facet_wrap(~iter, ncol=ncol, nrow=nrow)
  }
}

