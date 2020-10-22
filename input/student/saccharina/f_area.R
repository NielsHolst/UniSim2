m1 = 0.1085
m2 = 0.03
A0 = 6
farea = function(A) {
  (m1*exp(-(A/A0)^2) + m2)/m2
}

x = (0:200)/10
M = data.frame (
  A = x,
  farea = farea(x)
)

ggplot(M, aes(A, farea)) +
  geom_line(color="red") +
  theme_minimal()

1 + m2/m1
