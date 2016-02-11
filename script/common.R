library(ggplot2)
library(lubridate)
library(plyr)
library(reshape2)

read_unisim_output = function() {

	column_info = function() {
		sip = read.table(output_file_path, header=TRUE, sep="\t", stringsAsFactors=FALSE, nrows=3)
		Rformat = unlist(sip[1,])
		axis = unlist(sip[2,])
		page = unlist(sip[3,])
		ix_date = (Rformat == "ymd") | (Rformat == "hms") | (Rformat == "ymdhms")
		read_format = Rformat
		read_format[ix_date]  = "character"
		Rformat[!ix_date]  = NA
		list(col_names=colnames(sip), Rformat=Rformat, read_format=read_format, axis=axis, page=page)
	}

	# Extract column info
	cl = column_info()

	# Read data frame
	U = read.table(output_file_path, header=FALSE, sep="\t", colClasses = cl$read_format, skip=4)
	colnames(U) = cl$col_names

	# Convert date/time columns
	ix_date = which(!is.na(cl$Rformat))
	if (length(ix_date) > 0 ) {
		for (i in 1:length(ix_date)) {
			ix = ix_date[i]
			U[,ix] = parse_date_time(U[,ix], c(cl$Rformat[ix]))
		}
	}
	
	# Vector of y-columns and of shared columns, i.e. those that are not y-columns
	y_columns = which(cl$axis=="y")
	shared_columns = which(cl$axis!="y")

	# Vector of unique, non-NA page names, default to one page called 'Output'
	if (all(is.na(cl$page[y_columns])))  cl$page[y_columns] = "Output"
	pages = unique(cl$page)
	pages = pages[!is.na(pages)]
	
	
	# Create a list of data frames, one for each page
	L = alply(pages, 1, 
			function(page_name) { 
				cols = c(shared_columns, which(cl$page==page_name))
				V = data.frame(U[,cols])
				colnames(V)=colnames(U)[cols]
				V 
			}
		)
	
	# Melt each data frame in the list
	M = llply(L, function(X) melt(X, id.vars=colnames(U)[shared_columns]))
	
	# List of melted data frames, one for each page, and name of x-axis
	list(pages=M, x_name=colnames(U)[cl$axis=="x"])
}

create_unisim_plots = function() {
	L = read_unisim_output()
	llply(L$pages, 
		function(M) {
			ggplot(M, aes_string(x=L$x_name, y="value",colour="variable")) +
			geom_line() 
		}
	)
}

print_unisim_plots = function() {
	P = create_unisim_plots()
	n = length(P)
	if (n>0) {
		print(P[[1]])
		if (n>1) {
			for (i in 2:n) {
				print(P[[i]])
				windows()
			}
		}
	}
}

print_unisim_plots()



