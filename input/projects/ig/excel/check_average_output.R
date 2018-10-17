rm(list=ls(all=TRUE))
graphics.off()

library(plyr)

setwd("C:/Users/au152367/Documents/ig/output")

obs2m = read.table("DVV_Input_data_2min.txt", header=TRUE, sep="\t")
obs1h = read.table("DVV_Input_data_1h.txt", header=TRUE, sep="\t")

obs2m$Id = as.numeric(rownames(obs2m))%/%30

avg = function(M) {
  n = ncol(M)
  R = colSums(M[,3:(n-1)])/30
  # R$dateTime = M$dateTime[15]
  R
}

# M = subset(obs2m, Id==0)
# avg(M)

obs2m_avg = ddply(obs2m, .(Id), avg)
n = nrow(obs2m_avg)
obs2m_avg = obs2m_avg[-n,]

nrow(obs1h)
nrow(obs2m_avg)

plot(obs2m_avg$outdoorsT, obs1h$outdoorsT)
