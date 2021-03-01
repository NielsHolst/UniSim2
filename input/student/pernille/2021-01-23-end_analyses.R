sim = read_output(output_file_name)
sim$Location = factor(sim$iteration)
levels(sim$Location) = c("Campinas", "Itai", "Londrina", "Rondonia","SaoPaulo", "CostaRica")

sim$DayNum = yday(sim$date)

to_date = function(x) {
  dmy("1/9/2002") + days(round(x))
}

stats = function(df) {
  f = approxfun(df$outflowTotal, df$DayNum)
  data.frame(From = to_date(f(10)), 
             Median = to_date(f(50)), 
             To = to_date(f(90)),
             Duration = to_date(f(90)) - to_date(f(10)))
}

A = ddply(sim, .(Location), stats)
print(A)

P = ggplot(A) +
  geom_point(aes(y=Location, x=Median, colour=Location), size=4) +
  geom_errorbarh(aes(y=Location, x=Median, xmin=From, xmax=To, 
                     height=0, colour=Location), size=1.3) +
  xlab("CBB flying") + ylab("") +
  theme(legend.position="none")

x11(5,3)
print(P)

figures <- function(df) {
  Pages = list(Page = list(Grob=P, Width=5, Height=3))
}
