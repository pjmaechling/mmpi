clear;
g = [1 6 8 10:13 26 51 101]; % labels for x values
ny = 32; % Number of y values
nID = [1:10]; % Actual labels required on the x axis;
% The two arrays below are the values you need to plot
yAvg = rand(length(g),32); % Average of a quantity
yStdDev = rand(length(g),32)*0.15; % Standard deviation of the quantity

bar3(1:ny,yAvg') ; % Plot the 3D bar

% Put some labels
xlabel('X label', 'FontSize', 14);
ylabel('Y label', 'FontSize', 14);
zlabel('Some proportion', 'FontSize', 14);
title('3D bar plot with error bars', 'FontSize', 14);

% Set custom ticks and tick labels
set(gca, 'Xtick', nID);
set(gca, 'xTickLabel', g(nID));
colormap('white');

%Now plot the standard deviations as the line on top of the bars
hold on;
for i = 1:ny,
for j = 1:length(g),
z35 = yAvg(j,i):yStdDev(j,i)/20:yAvg(j,i)+yStdDev(j,i);
x35(1:length(z35)) = i;
y35(1:length(z35)) = j;
plot3(y35, x35, z35,'r-')
clear x35; clear y35; clear z35;
end
end
hold off
axis tight; 