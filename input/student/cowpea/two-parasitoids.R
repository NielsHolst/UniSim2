a = 0.8
b = 0.3

p = c(
  a_only = a*(1-b),
  b_only = b*(1-a),
  a_b_both = a*b,
  none = (1-a)*(1-b)
)
p
sum(p)

p = unname(p)
net = c(
  a = p[1] + p[3]/2,
  b = p[2] + p[3]/2,
  total = sum(p[1:3])
)
net
