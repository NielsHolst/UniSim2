
balance = function(Tin, T, Tout) {
  T0 = 273.15
  Tin  = Tin  + T0
  T    = T    + T0  
  Tout = Tout + T0
  if (Tin < Tout) {
    0
  } else  {
    (Tin - T)^4 - (T - Tout)^4
  }
}

balance(20, 12, 0)
balance(20, 10, 0)

