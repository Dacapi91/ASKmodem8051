%% GenerateSinTable

% Este script dibuja la sinusoide portadora para obtener los valores que
% se graban en la memoria del 8051.

A=128;
N=12;
Ts=42e-6;
fc = 1/(N*Ts);
i=1:N;
y = A*sin(2*pi.*i/N)+A; 
y=uint8(y);
t=0:Ts:Ts*(N-1); 
plot(t,y,'s-'); title('Sinusoide portadora'); xlabel('Tiempo (s)');
ylabel('Valores de la tabla');grid on;
disp(y);

