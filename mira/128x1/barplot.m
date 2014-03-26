%
% Load a set of communication data
load ammpi_128_64_100_comm.dat
Z = ammpi_128_64_100_comm;
h = bar3(Z);
for i = 1:length(h)
    zdata = ones(6*length(h),4);
    k = 1;
    for j = 0:6:(6*length(h)-6)
        zdata(j+1:j+6,:) = Z(k,i);
        k = k+1;
    end
    set(h(i),'Cdata',zdata)
end
colormap jet
colorbar
view(-118.0,35.0)
axis tight
daspect([1,1,max(max(Z))/5])
hidden off
camzoom(1.1)
xlabel('Processor Rank')
ylabel('Processor Rank')
zlabel('MPI Latency (usecs)')
title('MPI Comm Latency')
print -dpng comm_angle
view (-90,90)
axis tight
daspect([1,1,10])
hidden off
camzoom(1.1)
print -dpng comm_top
%
% Load a set of variable data
%
load ammpi_128_64_100_var.dat
Z = ammpi_128_64_100_var;
h = bar3(Z);
for i = 1:length(h)
    zdata = ones(6*length(h),4);
    k = 1;
    for j = 0:6:(6*length(h)-6)
        zdata(j+1:j+6,:) = Z(k,i);
        k = k+1;
    end
    set(h(i),'Cdata',zdata)
end
colormap jet
colorbar
view(-118.0,35.0)
axis tight
daspect([1,1,max(max(Z))/5])
hidden off
%lighting none
camzoom(1.1)
xlabel('Processor Rank')
ylabel('Processor Rank')
zlabel('MPI Latency Variability (StdDev)')
title('MPI Comm Latency Variability')
print -dpng vara
view (-90,90)
axis tight
daspect([1,1,10])
hidden off
camzoom(1.1)
%lighting flat
print -dpng vart