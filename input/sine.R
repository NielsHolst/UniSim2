sineWaveDown = function(x, x0, x1, y0, y1) {
    xr = ((x-x0)/(x1-x0)-0.5)*pi;
    y0 + (y1-y0)*(sin(xr)+1.)/2.;
}

sineWaveUp = function(x, x0, x1, y0, y1) {
    y0 + y1 - sineWaveDown(x, x0, x1, y1, y0);
}

sineWaveDown((0:10)/10, 0, 1, 0.9, 0.5)
sineWaveUp((0:10)/10, 0, 1, 0.5, 0.9)

