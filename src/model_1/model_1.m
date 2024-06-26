% Define the transfer function variable 's'
s = tf('s');

% PID controller parameters
k = 1000;
ki = 20;
kd = 60000;

% Plant transfer function (system to be controlled)
G = (0.00000961) / (s^2 + 0.002*s + 0.00000961);

% PID controller transfer function
Gc = k + (ki/s) + (kd*s);

% Closed-loop system with PID feedback
sys_cl = feedback(G * Gc, 1);

% Time vector for simulation
t = 0:0.01:1000;

% Step response of the closed-loop system
[y, t] = step(sys_cl, t);

% Plot the step response
figure;
plot(t, y);
grid on;

% Graph labels and title
title('Step Response of Closed-Loop System with PID Controller');
xlabel('Time (s)');
ylabel('Amplitude');

% Calculate and display maximum overshoot (Mp)
Mp = (max(y) - 1) * 100;
disp(['Maximum Overshoot (Mp): ', num2str(Mp), '%']);

% Discretize the PID controller using Tustin's method with a sampling time of 0.01 seconds
discr = c2d(Gc, 0.01, 'tustin');
