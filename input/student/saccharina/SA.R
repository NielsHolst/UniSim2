setwd("/Users/au152367/Documents/PAPERS/2021/saccharina")
load("sim-SA.Rdata")

colnames(sim) = unique_names(colnames(sim))

bestYield = order(sim$yieldSumSq)
bestPctN  = order(sim$pctNSumSq)
bestPctC  = order(sim$pctCSumSq)

head(sim[bestYield,])
head(sim[bestPctN,])
head(sim[bestPctC,])

n = 100
sim[intersect(bestYield[1:n], bestPctN[1:n]),]


intersect(intersect(bestYield[1:n], bestPctN[1:n]), bestPctC[1:n])


