library(stringr)
sim = read_output(output_file_name)
plot_all(sim)

reorder_levels = function(the_factor, new_order) {
  factor(the_factor,levels(the_factor)[new_order])
}

colour_first_order = "#66c2a5"
colour_total       = "#fc8d62"

input_columns = 2:(sobol_k+1)
input_names = colnames(sim)[input_columns]
input_names = str_replace(input_names, "\\.end", "")
input_names

output_column = function(ix_output) {
  sobol_k + ix_output + 1
}
output_names = colnames(sim)[output_column(1):ncol(sim)]

rows = function(offset, nrows=NA) {
  n = if (is.na(nrows)) sobol_N else nrows
  from = offset*sobol_N + 1
  to = from + n - 1
  from:to
}

A_rows = function(nrows=NA) rows(0,nrows)
B_rows = function(nrows=NA) rows(1,nrows)
C_rows = function(i, nrows=NA) rows(i+1,nrows)
C_rows_all = function(nrows=NA) {
  v = {}
  for (i in 1:sobol_k) v = c(v, C_rows(i, nrows))
  v
}

yA = function(ix_output, nrows=NA) {
  as.matrix( sim[A_rows(nrows), output_column(ix_output)] )
}
# sim
# yA(1)
# yA(2)

yB = function(ix_output, nrows=NA) {
  as.matrix( sim[B_rows(nrows), output_column(ix_output)] )
}
# yB(1)
# yB(2)

yC = function(ix_output, nrows=NA) {
  n = if (is.na(nrows)) sobol_N else nrows 
  matrix(sim[C_rows_all(nrows), output_column(ix_output)], nrow=n, ncol=sobol_k)
}
# yC(1,2)
# yC(2)

sobol_inputs = function(ix_output, nrows=NA) {
  n = if (!is.na(nrows) & nrows>sobol_N) sobol_N else nrows 
  list(yA = yA(ix_output, n), 
       yB = yB(ix_output, n), 
       yC = yC(ix_output, n))
}

bootstrap = function(sobol_inputs) {
  rows = sample(1:sobol_N, sobol_N, TRUE)
  list(yA = matrix(sobol_inputs$yA[rows,], nrow=sobol_N), 
       yB = matrix(sobol_inputs$yB[rows,], nrow=sobol_N), 
       yC = matrix(sobol_inputs$yC[rows,], nrow=sobol_N))
}

sobol_indices = function(y) {

  sum_sq = function(a,b) {
    sum( (a-b)^2 )
  } 

  mult = function(a,b) {
    sum(a*b)/N
  } 
  
  Si = function(i) {
    (mult(yA, yC[,i]) - f02) / V
  }

  STi = function(i) {
    1 - (mult(yB, yC[,i]) - f02) / V
  }
  
  Si_Jansen = function(i) {
    1 - sum_sq(yA, yC[,i])/2/N/V
  }

  STi_Jansen = function(i) {
    sum_sq(yB, yC[,i])/2/N/V
  }

  yA = y$yA
  yB = y$yB
  yC = y$yC
  N = nrow(yA)
  
  # Use both matrices to compute f02 and V
  # A. Saltelli in email 29-11-2018: As you cannot use mixed matrices for V and f0,
  # you can either use the estimates corresponding to A or those corresponding to B or both. 
  # When you use e.g. both you double the number of elementary effects and hence you improve you estimate.
  yAB = c(yA, yB)
  f02 = mean(yAB)^2
  V = mult(yAB,yAB)/2 - f02

  rbind(
    data.frame(
      Input = input_names,
      Measure = "FirstOrder",
      Effect  = sapply(1:sobol_k, Si_Jansen)
    )
    ,
    data.frame(
      Input = input_names,
      Measure = "Total",
      Effect = sapply(1:sobol_k, STi_Jansen) 
    )
  )  
}

plot_against_sample_size = function() {

  f = function(combi) {
    M = sobol_indices( sobol_inputs(combi$ix_output, combi$N) )
    M$Output = output_names[combi$ix_output]
    M
  }
  g = function(m) {
    data.frame(Effect=sum(m$Effect))
  }

  combinations = expand.grid(ix_output=1:length(output_names), N=sobol_N/10*(1:10))
  M = adply(combinations, 1, f)[,-1]

  Msum = ddply(M, .(Output, Measure, N), g)
  Msum$Input = "Sum"
  M = rbind(M,Msum)

  totals = subset(M, Measure=="Total" & Output==output_names[1] & N==max(N))
  alphabetic_ordering = order(totals$Input)
  totals = totals[alphabetic_ordering, ]
  effect_ordering = order(totals$Effect, decreasing=TRUE)
  M$Input = reorder_levels(M$Input, effect_ordering)

  ggplot(M, aes(x=N, y=Effect, colour=Measure)) +
    geom_line(size=1) +
    ylim(-0.05,NA) +
    geom_hline(yintercept=0, colour="grey") +
    geom_hline(yintercept=1, colour="grey", size=1) +
    facet_grid(Output~Input) +
    guides(color=guide_legend(reverse=TRUE)) +
    scale_colour_manual(values = c(colour_first_order, colour_total)) + 
    theme_classic() +
    theme(
      strip.background = element_rect(fill="grey90", color=NA)
    ) 
}
open_plot_window(9,4)
print( plot_against_sample_size() )

plot_effects = function(ix_output) {

  g = function(m) {
    data.frame(Effect=sum(m$Effect))
  }

  M = sobol_indices( sobol_inputs(ix_output) )
  Msum = ddply(M, .(Measure), g)
  Msum$Input = "Sum"
  M = rbind(M,Msum)

  totals = subset(M, Measure=="Total")
  alphabetic_ordering = order(totals$Input)
  totals = totals[alphabetic_ordering, ]
  effect_ordering = order(totals$Effect, decreasing=FALSE)
  M$Input = reorder_levels(M$Input, effect_ordering)

  ggplot() +
    geom_segment(aes(x=0, xend=Effect, y=Input, yend=Input), subset(M, Measure=="Total"),      colour=colour_total, size=2) +
    geom_segment(aes(x=0, xend=Effect, y=Input, yend=Input), subset(M, Measure=="FirstOrder"), colour=colour_first_order, size=2) +
    geom_vline(xintercept=0, colour="grey") +  
    geom_vline(xintercept=1, colour="grey", size=1) +  
    scale_x_continuous("Effect", breaks=(0:8)/4) +
    theme_classic() +
    theme(
      axis.ticks.y = element_blank(),
      axis.line.y = element_blank(),
      strip.background = element_rect(fill="grey90", color=NA)
    ) +
    labs(title=paste("Sensitivity of", output_names[ix_output]))
}

open_plot_window(9,9)
grid.arrange(grobs = list(plot_effects(1), plot_effects(2)))
     
bootstrap_effects = function(ix_output, n) {

  f = function(i) {
    g = function(m) {
      data.frame(Effect=sum(m$Effect))
    }

    M = sobol_indices( bootstrap(inputs) )
    Msum = ddply(M, .(Measure), g)
    Msum$Input = "Sum"
    rbind(M,Msum)
  }
  
  inputs = sobol_inputs(ix_output)
  M = adply(1:n, 1, f)
  colnames(M)[1] = "Sample"
  M$Output = output_names()[ix_output]
  M
}

statistics = function(B, num_digits=2) {
  M = ddply(B, .(Input, Measure), summarize,
    EffectMean       = mean(Effect),
    EffectSE         = sum((EffectMean-Effect)^2),
    LowerPercentile  = quantile(Effect, 0.025),
    HigherPercentile = quantile(Effect, 0.975),
    P = length(which(Effect<=0))/length(Effect)
  )
  M$EffectMean = round(M$EffectMean, num_digits)
  M$EffectSE = round(sqrt(M$EffectSE/(nrow(B)-1)), num_digits+1)
  M$LowerPercentile = round(M$LowerPercentile, num_digits)
  M$HigherPercentile = round(M$HigherPercentile, num_digits)
  M$Sig = "ns"
  M$Sig[M$P<=0.05]  = "*"
  M$Sig[M$P<=0.01]  = "**"
  M$Sig[M$P<=0.001] = "***"
  M$P = round(M$P, 4)

  totals = subset(M, Measure=="Total")
  alphabetic_ordering = order(totals$Input)
  totals = totals[alphabetic_ordering, ]
  effect_ordering = order(totals$EffectMean, decreasing=TRUE)
  effect_ordering
    
  f = function(x) c(x,x-1)
  row_numbers = c(sapply(2*effect_ordering, f))
  M = M[row_numbers,]
  rownames(M) = NULL
  # M = cbind(Output=rep(B$Output[1], nrow(M)), M)
  M
}

B = adply(1:2, 1, bootstrap_effects, n=1000)
S = ddply(B, .(Output), statistics)



# B1 = bootstrap_effects(1, 10000)
# statistics(B1)

# plot_bootstrap_effects = function(B) {
    
  # ggplot(B, aes(y=Effect, colour=Measure, fill=Measure)) +
    # geom_boxplot(alpha=0.3) +
    # theme_classic() +
    # facet_wrap(~Input, ncol=length(levels(M$Input)))
   
  # # ggplot(B, aes(colour=Measure)) +
    # # geom_segment(aes(x=LowEffect, xend=HighEffect, y=Input, yend=Input)) +
    # # geom_point(aes(x=MeanEffect, y=Input)) +
    # # theme_classic() +
    # # facet_wrap(~Measure, ncol=length(levels(M$Input)))
# }



  
  



     