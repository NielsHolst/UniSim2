M = sim[,c("dateTime", "outdoorsLight", "indoorsTotalPar", "actLightPar")]
M$outdoorsPar = M$outdoorsLight*2.3
M = subset(M, hour(dateTime)>11 & hour(dateTime)<13 & actLightPar==0)
head(M)

ggplot(M, aes(outdoorsPar, indoorsTotalPar)) +
  geom_point(colour=orange, alpha=0.3) +
  geom_abline(slope=1,   intercept=0, colour=brown, size=1) +
  geom_abline(slope=0.89, intercept=0, colour=brown, size=1, linetype="dashed") +
  geom_vline(xintercept=500*2.3, colour=red, size=1)
  
