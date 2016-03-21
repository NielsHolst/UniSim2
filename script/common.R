library(ggplot2)
library(gridExtra)
library(lubridate)
library(plyr)
library(reshape2)

read_unisim_output = function(file_path) {

	column_info = function(file_path) {
		sip = read.table(file_path, header=TRUE, sep="\t", stringsAsFactors=FALSE, nrows=1)
		Rformat = unlist(sip[1,])
		ix_date = (Rformat == "ymd") | (Rformat == "hms") | (Rformat == "ymdhms")
		read_format = Rformat
		read_format[ix_date]  = "character"
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

unisim_plot_facetted = function(df, id.x, cols, ncol, nrow) {
	if (!(id.x %in% cols)) cols = c(id.x, cols)
	M = melt(df[ ,cols], id.vars=id.x, value.name="Value", variable.name="Variable")
	ggplot(M, aes_string(x=id.x, y="Value", color="Variable")) +
		geom_line(size=1.1) +
		theme(legend.position="none") +
		xlab("") + ylab("") +
		facet_wrap(~Variable, ncol=ncol, nrow=nrow, scales="free_y")
}

unisim_plot_merged = function(df, id.x, cols) {
	if (!(id.x %in% cols)) cols = c(id.x, cols)
	M = melt(df[ ,cols], id.vars=id.x, value.name="Value", variable.name="Variable")
	ggplot(M, aes_string(x=id.x, y="Value", color="Variable")) +
		geom_line(size=1.1) +
		xlab("") + ylab("")
}
