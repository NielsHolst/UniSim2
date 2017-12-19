  library(plyr)
  rm(list=ls(all=TRUE))

  # Set up attack rate matrix
  A = matrix(rep(0,25),5)
  pop_names = c("Y1","Y2","Y3","X1","X2")
  colnames(A) = pop_names
  rownames(A) = pop_names

  A[1,4] = 0.04
  A[2,4] = 0.07
  A[3,4] = 0.01

  A[1,5] = 0.04
  A[2,5] = 0.03
  A[3,5] = 0.08
  A

  # Set up gain matrix
  G = matrix(1,5,5)

  G[1,4] = 1
  G[2,4] = 2.5
  G[3,4] = 2.5

  G[1,5] = 1
  G[2,5] = 4.7
  G[3,5] = 4.7
  G

  # Set up population demand vector
  d = c(0.6, 5.8, 2.4, 0, 0) # per capita demand rates
  d = matrix(d)
  rownames(d) = pop_names
  colnames(d) = "d"
  d

  # Set up population density vector
  N = c(5, 10, 2, 15, 20)
  names(N) = pop_names
  N

  # Population demands
  D = d*matrix(N)                       
  colnames(D) = "D"
  D

  # Set time step
  Dt = 1

  #
  # Many attackers-one resource
  #

  # Search rate for resource (j) pooled on all attackers
  func_s_pooled = function() {
    per_col = function(j) {
      1-exp(-sum(A[,j]*N))
    }
    S = aaply(1:5, 1, per_col)
    names(S) = pop_names
    S
  }
  s_pooled = func_s_pooled()
  round(s_pooled,3)

  # Search rate for resource (j) by attacker (i) , first approximation
  func_s_approx = function() {
    per_row = function(i) {
      1-exp(-A[i,]*N[i])
    }
    S = aaply(1:5, 1, per_row)
    S = data.frame(S)
    rownames(S) = pop_names
    S
  }
  s_approx = func_s_approx()
  round(s_approx,3)
  s_approx_with_sum = rbind(s_approx, colSums(s_approx))
  rownames(s_approx_with_sum)[6] = "Sum"
  round(s_approx_with_sum,3)

  # Search rate for resource (j) by attacker (i), final
  func_s_final = function() {
    per_col = function(j) {
      sum_approx = sum(s_approx[,j])
      if (sum_approx==0) {
        rep(0,5)
      } else {
        s_approx[,j]/sum_approx*s_pooled[j]
      }
    }
    S = aaply(1:5, 1, per_col)
    S = t(S)
    S = data.frame(S)
    rownames(S) = pop_names
    colnames(S) = pop_names
    S
  }
  s = func_s_final()
  round(s,3)
  s_with_sum = rbind(s, colSums(s))
  rownames(s_with_sum)[6] = "Sum"
  round(s_with_sum,3)

  #
  # One attacker-many resources
  #

  # Acquistion by attacker (j) pooled on all resources
  func_mDX_pooled = function() {
    per_row = function(i) {
      A = D[i]*(1 - exp(-sum(s[i,]*G[i,]*N)/D[i]))
      A[D[i]==0] = 0
      A
    }
    A = aaply(1:5, 1, per_row)
    A = matrix(A)
    rownames(A) = pop_names
    colnames(A) = "mDX"
    A
  }
  mDX_pooled = func_mDX_pooled()
  mDX_pooled_with_D = cbind(mDX_pooled, D)
  round(mDX_pooled_with_D,2)

  # Acquistion by attacker (j) of resource (i), first approximation
  func_mDX_approx = function() {
    per_row = function(i) {
      A = D[i]*(1 - exp(-s[i,]*G[i,]*N/D[i]))
      A[D[i]==0] = 0
      A
    }
    A = adply(1:5, 1, per_row)
    rownames(A) = pop_names
    colnames(A) = pop_names
    A
  }
  mDX_approx = func_mDX_approx()
  mDX_approx_with_sum = cbind(mDX_approx, rowSums(mDX_approx))
  colnames(mDX_approx_with_sum)[6] = "Sum"
  round(mDX_approx_with_sum, 2)

  # Acquistion by attacker (j) of resource (i), final
  func_mDX = function() {
    per_row = function(i) {
      sum_mDX_approx = sum(mDX_approx[i,])
      A = mDX_approx[i,]/sum_mDX_approx*mDX_pooled[i]
      A[sum_mDX_approx==0] = 0
      A
    }
    A = adply(1:5, 1, per_row)
    rownames(A) = pop_names
    colnames(A) = pop_names
    A
  }
  mDX = func_mDX()
  S = cbind(mDX, rowSums(mDX), D)
  colnames(S)[6] = "Supply"
  round(S,2)

  mN = mDX/G
  mN = rbind(mN, colSums(mN), N)
  rownames(mN)[6:7] = c("Loss", "N")
  round(mN,2)

