T = 12
rh = 80

T0 = 273.15
R = 8.314
Mwater = 18.016e-3

svp = function(temperature) {
    T = temperature + T0;
    133.32*exp((1.0887*T - 276.4)/(0.0583*T - 2.1938));
}

ahFromVp = function(temperature, vp) {
    vp*Mwater/(R*(temperature + T0));
}

sah = function(temperature) {
    ahFromVp(temperature, svp(temperature));
}

ahFromRh = function(temperature, rh) {
    rh/100*sah(temperature);
}

svp(T)
sah(T)
ahFromRh(T, rh)
