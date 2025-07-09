n=0:1:3599;
k=2*pi/3600;
a=128*sin(k*n)+128;
a=uint16(a);
dlmwrite('dac_sinWave.c',a);
plot(n,a);