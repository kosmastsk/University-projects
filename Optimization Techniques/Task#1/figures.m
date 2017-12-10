%Define the functions and create their plots
%for xE[0,6]
f1 = @(x) (x-2).^2+(x-5).^2;
f2 =@(x) (x-4).^2+cos(x);
f3=@(x) sqrt(x+1)+(x.^4-2).*log(x+1);
x=0:0.1:6;
figure;
plot(x,f1(x));
grid on;
xlabel('x');
ylabel('f1(x)');
figure;
plot(x,f2(x));
grid on;
xlabel('x');
ylabel('f2(x)');
figure;
plot(x,f3(x));
grid on;
xlabel('x');
ylabel('f3(x)');