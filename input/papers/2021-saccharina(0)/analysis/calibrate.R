# Set the working directory to point to your input folder
setwd("/Users/au152367/Documents/QDev/UniSim2/input")
source("papers/2021-saccharina/analysis/setup.R")
load(  "papers/2021-saccharina/analysis/saccharina-SA-complete_0009.Rdata")

simplify_col_names

# Find best fit for each sum of squares
minArea  = min(sim$areaSumSq)
minYield = min(sim$yieldSumSq)
minPctN  = min(sim$pctNSumSq)
minPctC  = min(sim$pctCSumSq)

minArea 
minYield
minPctN 
minPctC 

# Scale eacn sum of squares to give them all equal weight 
sim$areaSumSqSc  = sim$areaSumSq  - minArea
sim$yieldSumSqSc = sim$yieldSumSq - minYield
sim$pctNSumSqSc  = sim$pctNSumSq  - minPctN
sim$pctCSumSqSc  = sim$pctCSumSq  - minPctC

# Best over all fit. We ignore yield as we consider the observations much more uncertain than the others.
sim$allSc = 10^(sim$areaSumSqSc) + 10^(sim$pctNSumSqSc) + 10^(sim$pctCSumSqSc)

# List best 20 fits
M = sim[order(sim$allSc),]
head(M,20)
# Show parameters of best fit
m = M[1,2:19]
t(round(m,6))

# # List best 20 fits for area only
# M = sim[order(sim$areaSumSq),]
# head(M,20)
# # Show parameters of best fit for area only
# m = M[1,2:19]
# t(round(m,6))


