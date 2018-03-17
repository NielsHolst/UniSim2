library(QuantifQuantile)
set.seed(642516)
n <- 1000
X <- matrix(runif(n*2, -2, 2), ncol = n)
Y <- apply(X^2, 2, sum) + rnorm(n)
testN <- seq(40, 90, by = 10)
B <- 20
tildeB <- 15
res <- QuantifQuantile.d2(X, Y, testN = testN, B = B, tildeB = tildeB)
plot(res, ise = TRUE)


X = cbind(sim$critResistance, sim$critDensity)
X = as.matrix(X)
X = t(X)
dim(X)
Y = sim$coverage.maxValue.end
length(Y)
res <- QuantifQuantile.d2(X, Y)



dim(X)
length(Y)
names(res)
dim(res$x)
res$alpha
dim(res$fitted.values)
dim(res$alpha)

