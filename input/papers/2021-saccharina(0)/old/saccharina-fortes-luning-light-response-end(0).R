source(paste0(box_script_folder, "/end-functions.R"))
graphics.off()

obs = data.frame(
  irradiation =
    c(0, 5,10,20,30,70,110,150,250),
  rLength =
    c(0, 0.8,1.8, 3, 3.3,4.9,5.8,5.5,2.5)/100
)

M = melt(sim[,c("irradiation","rLength", "C", "N")], id.vars="irradiation")

A = ggarrange(
  ggplot(M, aes(irradiation,value)) +
    geom_point(colour=blue) +
    # geom_point(color=brown, size=2, data=obs) +
    # geom_line (color=brown, size=1, data=obs) +
    geom_smooth(color=brown) +
    labs(title="Response curves") +
    facet_wrap(~variable, scales="free") +
    theme_bw() 
  ,
  ggplot(sim, aes(irradiation,rLength)) +
    geom_point(colour=blue) +
    geom_point(color=brown, size=2, data=obs) +
    geom_line (color=brown, size=1, data=obs) +
    labs(title="Compared with obs from Fortes & Lüning (1980)") +
    theme_bw() 
  ,
  ncol=1
)  
print(A)

model = lm(rLength~poly(irradiation, 2, raw=TRUE), data=tail(obs,3))
a = coef(model)[3]
b = coef(model)[2]
c = coef(model)[1]
D = b^2 - 4*a*c
xintercept = (-b-sqrt(D))/2/a
xintercept

g = function(x) {
  a*x^2 + b*x + c
}
x = 110:ceiling(xintercept)
fit = data.frame(
  irradiation = x,
  rLength = g(x)
)

ggplot(tail(obs,3), aes(irradiation, rLength)) +
  geom_point(color=brown, size=2) +
  geom_line(colour=blue, data=fit) +
  ylim(0, NA) +
  xlim(NA, xintercept) +
  theme_bw() 
  
model = lm(log(rLength) ~ log(irradiation), obs[3:7,])
slope = coef(model)[2]
intercept = coef(model)[1]

f = function(x) {
  logy = slope*log(x) + intercept
  exp(logy)
}
x = 1:110
fit = data.frame(
  irradiation = x,
  rLength = f(x)
)
  
ggplot(obs[3:7,], aes(irradiation, rLength)) +
  geom_point(color=brown, size=2) +
  geom_line(colour=blue, data=fit) +
  # ylim(0, NA) +
  # xlim(0,110) +
  theme_bw() 
  
ggplot(obs[-1,], aes(irradiation, rLength)) +
  geom_point(color=brown, size=3) +
  geom_smooth(method="lm", formula=y~poly(x, 2, raw=TRUE), colour=blue) +
  theme_bw() 
