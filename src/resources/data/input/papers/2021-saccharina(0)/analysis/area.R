a = 0.116
b = 0.114

A = function(W) {
  W/(a*W +b)
}

W = function(A) {
  b*A/(1-A*a)
}

A(1.05)
x = A(2.29)
W(x)

W(0.007189)
