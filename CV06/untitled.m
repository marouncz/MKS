clear all;
close all;
clc

data = csvread("ntc.csv");
% data(:, 3) is voltage
data(:, 3) = data(:, 2)./(data(:, 2) + 10).*3.3;
% data(:, 4) is 10bit ad value
data(:, 4) = data(:, 3)./3.3.*1024;

plot(data(:, 4), data(:, 1));
p = polyfit(data(:, 4), data(:, 1), 10);

ad2 = 0:1023;
t2 = round(polyval(p, ad2), 1);
hold on;
plot(ad2, t2, 'r');

dlmwrite('data.dlm', t2*10, ',')
