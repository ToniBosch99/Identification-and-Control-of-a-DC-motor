close all; clear
load('arduinodata.mat')
global u t y h
u=table2array(arduinodata(:,2));
y=table2array(arduinodata(:,3));
t=table2array(arduinodata(:,1))*1e-6;
h = mean(diff(t));
initial_params=[1 1 1 1 1];

optimized_params = fminsearch(@distance, initial_params);
disp('Optimized parameters:');
disp(optimized_params);

J=optimized_params(1);
b=optimized_params(2);
K=optimized_params(3);
R=optimized_params(4);
L=optimized_params(5);
