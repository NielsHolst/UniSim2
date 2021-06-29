source(paste0(box_script_folder, "/end-functions.R"))
graphics.off()

obs = data.frame(
  irradiation =
    c(0, 5,10,20,30,70,110,150,250),
  rLength =
    c(0, 0.8,1.8, 3, 3.3,4.9,5.8,5.5,2.5)/100
)

M = melt(sim[,c("irradiation","rLength", "C", "N")], id.vars="irradiation")

fit = function() {
  model = lm(rLength~poly(irradiation, 2, raw=TRUE), data=obs[-1,])
  a = coef(model)[3]
  b = coef(model)[2]
  c = coef(model)[1]
  D = b^2 - 4*a*c
  xintercept = (-b-sqrt(D))/2/a
  xintercept

  f = function(x) {
    a*x^2 + b*x + c
  }
  x = 0:ceiling(xintercept)
  data.frame(
    irradiation = x,
    rLength = f(x)
  )
}

A = ggarrange(
  ggplot(M, aes(irradiation,value)) +
    geom_point(colour=blue) +
    geom_smooth(color=brown) +
    labs(title="Response curves") +
    facet_wrap(~variable, scales="free") +
    theme_bw() 
  ,
  ggplot(sim, aes(irradiation,rLength)) +
    geom_point(colour=blue) +
    geom_point(color=brown, size=3, data=obs) +
    geom_line (color=brown, size=1, data=fit()) +
    labs(title="Compared with obs from Fortes & Lüning (1980)") +
    theme_bw() 
  ,
  ncol=1
)  
print(A)


ggplot(obs, aes(irradiation, rLength)) +
  geom_line(data=fit, colour=blue) +
  geom_point(color=brown, size=3) +
  geom_hline(yintercept=c(0,c), linetype="dashed") +
  geom_vline(xintercept=0, linetype="dashed") +
  # geom_smooth(method="lm", formula=y~poly(x, 2, raw=TRUE), colour=blue) +
  theme_bw() 
