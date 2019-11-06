library(ggplot2)
library(gridExtra)
library(lubridate)
library(plyr)
library(reshape2)
library(scales)
library(splines)
library(stringr)


if (!keepPages) graphics.off ()
if (!keepVariables) {
  del = ls(all=TRUE) 
  del = del[del!="box_script_folder"]
  rm(list=del)
}

is_rstudio = function() !is.na(Sys.getenv("RSTUDIO", unset = NA))
skip_formats = exists("output_skip_formats")

# See https://data-se.netlify.com/2018/12/12/changing-the-default-color-scheme-in-ggplot2/
unisim_colours = c('#e41a1c','#377eb8','#4daf4a','#984ea3','#ff7f00','#a65628','#f781bf','#999999')  
scale_colour_discrete = function(...) {
  scale_colour_manual(..., values = rep(unisim_colours, 10))
}

scale_linetype_discrete = function(...) {
  n = length(unisim_colours)
  scale_linetype_manual(..., values = rep(c(rep(1, n), rep(7, n)), 5))
}

check_num_colours = function(list_of_names) {
  unique_names = unique(unlist(list_of_names))
  n = length(unique_names)
  nmax = 10*length(unisim_colours)
  if (n > nmax) {
    stop(paste0("Too many variables in plot(",n," > ", nmax, ")\n", unique_names))
  }
}

update_geom_defaults("line", list(size=1))

reorder_levels = function(the_factor, new_order) {
  factor(the_factor,levels(the_factor)[new_order])
}

open_plot_window = function(width, height) {
  is_windows = (.Platform$OS.type == "windows")
  if (!is_rstudio()) {
    if (is_windows) windows(width=width, height=height) else
    X11(width=width, height=height, type="cairo") 
  }
}


unique_names = function(col_names) {

  f = function(x) {
    n = nrow(x)
    if (n>0) x[n,1] else NA
  }
  
  g = function(x) {
    if (is.na(x$LastDot)) {
      name = col_names[x$Row]
      suffix = NA
    } else {
      name = substring(col_names[x$Row], 1, x$LastDot-1)
      suffix = substring(col_names[x$Row], x$LastDot+1)
    }
    data.frame(Name=name, Suffix=as.character(suffix))
  }

  h = function(x) {
    data.frame(Count = sum(x$Name==M$Name))
  }
  
  F = function(x) {
    do_shorten = is.na(x$Suffix) | 
                 (x$Count==1 & x$Suffix %in% c("end", "value", "content"))  
    data.frame(
      UniqueName = 
        if (do_shorten) x$Name else paste(x$Name, x$Suffix, sep=".")
    )
  }
  
  locations = str_locate_all(col_names, "[.]")
  M = ldply(locations, f)
  colnames(M) = "LastDot"
  M$Row = 1:nrow(M)
  M = adply(M, 1, g)
  
  M = adply(M, 1, h)
  M = adply(M, 1, F)
  changed = any(as.character(col_names) != as.character(M$UniqueName))
  if (changed) unique_names(M$UniqueName) else M$UniqueName
}

read_output = function(file_path) {

  column_info = function(file_path) {
    sip = read.table(file_path, header=TRUE, sep="\t", stringsAsFactors=FALSE, nrows=1)
    Rformat = unlist(sip[1,])
    ix_date = (Rformat == "ymd") | (Rformat == "HMS") | (Rformat == "ymdHMS")
    read_format = Rformat
    read_format[ix_date]  = "character"
    names(read_format) = NULL
    Rformat[!ix_date]  = NA
    list(col_names=colnames(sip), Rformat=Rformat, read_format=read_format)
  }

  if (skip_formats) {
    U = read.table(file_path, header=TRUE, sep="\t")
  } else {
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
  }
  U
}

convert_logical = function(df) {
  ix_bool = which(sapply(df,class)=="logical")
  for (ix in ix_bool)
    df[,ix] = as.numeric(df[,ix])
  df
}

convert_factors = function(df, cols) {
  ix_factor = which(sapply(df[cols],is.factor))
  for (ix in ix_factor)
    df[cols[ix]][,1] = as.numeric(df[cols[ix]][,1])
  df
}

melted = function(df, id_x, id_iteration, cols) {
  df = convert_factors(df, cols)
  cols_id = c(id_x, id_iteration)
  cols_final = union(cols, cols_id)
  M = melt(convert_logical(df)[ ,cols_final], id.vars=cols_id, value.name="Value", variable.name="Variable")
  if (!is.null(id_iteration)) M$iter = factor(M[,id_iteration])
  M
}

has_iterations = function(M) {
  !is.null(M$iter) && length(unique(M$iter))>1
}

has_end = function(M) {
  any(endsWith(colnames(M), ".end"))
}

geom_deduced = function(M) {
  if (has_end(M)) geom_point() else geom_line()
}

Log10 = function(x) {
  y = rep(NA, length(x))
   y[x>0] = log10(x[x>0])
  y
}

ggplot_theme = function(fontSize) {
  if (fontSize==0) {
    NULL
  } else {
    sz = element_text(size = fontSize)
    sz1 = element_text(size = fontSize+1)
    theme(axis.text    = sz,
          axis.title   = sz,
          strip.text.x = sz,
          strip.text.y = sz,
          legend.title = sz1,
          legend.text  = sz
          )
  }
}

plot_facetted_one_x = function(df, id_x, id_iteration, cols, ytrans, ncol, nrow) {
  M = melted(df, id_x, id_iteration, cols)
  if (ytrans=="log10") M$Value = Log10(M$Value)
  hasIterations = has_iterations(M)
  onlyOneVariable = (length(cols) == 1)
  levels(M$Variable)  = unique_names(levels(M$Variable))

  color = if (hasIterations & onlyOneVariable) "iter" else "Variable"
  check_num_colours(M[color])
  P = ggplot(M, aes_string(x=id_x, y="Value", color=color)) +
    geom_deduced(df) +
    theme(legend.position="none")
  if (hasIterations | !onlyOneVariable) {
    P = P + xlab("") + ylab("")
  }
  
  if (hasIterations) {
    P + facet_grid(iter~Variable, scales="free_y") 
  } else if (!onlyOneVariable) { 
    P + facet_wrap(~Variable, ncol=ncol, nrow=nrow, scales="free_y")
  } else {
    P + ylab(cols[1])
  }
}

plot_merged_one_x = function(df, id_x, id_iteration, cols, ytrans, ncol, nrow) {
  M = melted(df, id_x, id_iteration, cols)
  if (ytrans=="log10") M$Value = Log10(M$Value)
  hasIterations = has_iterations(M)
  onlyOneVariable = (length(cols) == 1)
  levels(M$Variable)  = unique_names(levels(M$Variable))

  color = if (hasIterations & onlyOneVariable) "iter" else "Variable"
  check_num_colours(M[color])
  P = ggplot(M, aes_string(x=id_x, y="Value", color=color)) +
    geom_deduced(df) 
  if (onlyOneVariable) {
    P = P + xlab(id_x) + ylab(cols)
  } else {
    P = P + xlab("") + ylab("")
  }
  
  if (hasIterations & !onlyOneVariable) P = P + facet_wrap(~iter, ncol=ncol, nrow=nrow)
  P
}

meltxy = function(df, xvars, id_iteration, yvars) {
  if (length(xvars)==length(yvars) && all(xvars==yvars)) {
    print("xvars are equal to yvars:")
    print(xvars)
    stop("stopped")
  }
  M = melt(convert_logical(df), measure.vars=xvars, value.name="xValue", variable.name="xVariable")
  M = melt(M, id.vars=c(id_iteration,"xValue","xVariable"), measure.vars=yvars, value.name="ResponseValue", variable.name="Response")
  if (!is.null(id_iteration)) M$iter = factor(M[,id_iteration])
  M
}

plot_facetted_many_x = function(df, id_x, id_iteration, cols, ytrans, ncol, nrow) {
  M = meltxy(df, id_x, id_iteration, cols)
  if (ytrans=="log10") M$ResponseValue = Log10(M$ResponseValue)
  hasIterations = has_iterations(M)
  levels(M$Response)  = unique_names(levels(M$Response))

  color = if (hasIterations) "iter" else "Response"
  check_num_colours(M[color])
  ggplot(M, aes_string(x="xValue", y="ResponseValue", color=color)) +
    geom_deduced(df) +
    xlab("") + ylab("") +
    facet_grid(Response~xVariable, scales="free") 
}

plot_merged_many_x = function(df, id_x, id_iteration, cols, ytrans, ncol, nrow) {
  M = meltxy(df, id_x, id_iteration, cols)
  if (ytrans=="log10") M$ResponseValue = Log10(M$ResponseValue)
  hasIterations = has_iterations(M)
  onlyOneVariable = (length(cols) == 1)
  useIter = (hasIterations & onlyOneVariable) 
  levels(M$Response)  = unique_names(levels(M$Response))

  color = if (useIter) "iter" else "Response"
  check_num_colours(M[color])
  p = ggplot(M, aes_string(x="xValue", y="ResponseValue", color=color)) +
    geom_deduced(df) +
    xlab("")  + ylab("")
  p = p + if (useIter) facet_grid(Response~xVariable, scales="free") else facet_wrap(~xVariable, scales="free") 
  p
}

plot_facetted_filtered = function(df, id_x, cols, ytrans, ncol, nrow) {
  M = meltxy(df, id_x, NULL, cols)
  levels(M$Response)  = unique_names(levels(M$Response))
  levels(M$xVariable) = unique_names(levels(M$xVariable))
  if (ytrans=="log10") M$ResponseValue = Log10(M$ResponseValue)
  check_num_colours(M$Response)
  ggplot(M, aes(x=xValue, y=ResponseValue, color=Response)) +
    geom_deduced(df) +
    xlab("") + ylab("") +
    theme(legend.position="none") +
    facet_grid(Response~xVariable, scales="free")
}

plot_merged_filtered = function(df, id_x, cols, ytrans, ncol, nrow) {
  M = meltxy(df, id_x, NULL, cols)
  if (ytrans=="log10") M$ResponseValue = Log10(M$ResponseValue)
  levels(M$Response)  = unique_names(levels(M$Response))
  check_num_colours(M$Response)
  ggplot(M, aes(x=xValue, y=ResponseValue, color=Response)) +
    geom_deduced(df) +
    xlab("") + ylab("") +
    facet_wrap(~xVariable, scales="free")
}

plot_facetted = function(df, id_x, id_iteration, cols, ytrans, ncol, nrow) {
  if (!is.null(id_iteration) && id_iteration == "iteration.end")
  # if (!is.null(id_iteration) && max(df[id_iteration]) > 1)
    plot_facetted_filtered(df, id_x, cols, ytrans, ncol, nrow) else
    if (length(id_x) == 1) 
      plot_facetted_one_x( df, id_x, id_iteration, cols, ytrans, ncol, nrow) else
      plot_facetted_many_x(df, id_x, id_iteration, cols, ytrans, ncol, nrow)
}

plot_merged = function(df, id_x, id_iteration, cols, ytrans, ncol, nrow) {
  if (!is.null(id_iteration) && id_iteration == "iteration.end")
  # if (!is.null(id_iteration) && max(df[id_iteration]) > 1)
    plot_merged_filtered(df, id_x, cols, ncol, nrow) else
    if (length(id_x) == 1) 
      plot_merged_one_x( df, id_x, id_iteration, cols, ytrans, ncol, nrow) else
      plot_merged_many_x(df, id_x, id_iteration, cols, ytrans, ncol, nrow)
}

plot_density = function(df, ports, ncol, nrow) {
  M = df[ports]
  colnames(M) = unique_names(colnames(M))
  M = melt(M, value.name="Value", variable.name="Variable")
  ggplot(M, aes(x=Value, colour=Variable, fill=Variable)) +
    geom_density(alpha=0.3) +
    labs(y="") + 
    facet_wrap(~Variable, scales="free", ncol=ncol, nrow=nrow) +
    theme(
      axis.text.y = element_blank(),
      axis.ticks.y = element_blank(),
      legend.position="none"
    )
}

plot_histogram = function(df, ports, bins, ncol, nrow) {
  M = df[ports]
  colnames(M) = unique_names(colnames(M))
  M = melt(M, value.name="Value", variable.name="Variable")
  ggplot(M, aes(x=Value, colour=Variable, fill=Variable)) +
    geom_histogram(alpha=0.3, bins=bins) +
    labs(y="") + 
    facet_wrap(~Variable, scales="free", ncol=ncol, nrow=nrow) +
    theme(legend.position="none")    
}

plot_sobol_convergence = function() {
  plot_against_sample_size()
}

plot_sobol_indices = function(theme=NULL) {
  n_outputs = length(output_names())
  print(paste0("Bootstrapping (n=", sobol_B, ")..."))
  B = adply(1:n_outputs, 1, sobol_bootstrap, n=sobol_B)
  S = ddply(B, .(Output), sobol_statistics)
  print(S)
  dlply(subset(S, Input!="Sum"), .(Output), plot_effects, theme=theme)
}

