%diagram script creates the plot of the function f
%in order to create the image of the function that
%we are going to analyse
k=1;
for i=-3:0.01:3
    for j=-3:0.01:3
        x(k)=i;
        y(k)=j;
        z(k)=f(i,j);
        k=k+1;
    end
end
figure;
hold on;
xlabel('x');
hold on;
ylabel('y');
hold on;
zlabel('f(x,y)');
hold on;
grid on;
hold on;
plot3(x,y,z);