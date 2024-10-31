r = ntc.r;
t = ntc.t;
ad = (r ./ (r + 10)) * 2^10;

p = polyfit(ad,t,10);
ad2=0:1023;
t2 = round(polyval(p, ad2), 1);
plot(ad, t, 'bo'); %%aby to bylo jen v úseku definovanejch hodnot
hold on, plot(ad2, t2, 'r');
dlmwrite('data.dlm', t2*10, ',');