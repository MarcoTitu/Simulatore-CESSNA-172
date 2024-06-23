%% POST PROCESSING: SIM172

clc
clear
close all


%% Read files
FilePath_c=strcat(pwd,"\Input\commandHistory2.txt");
dataTab_c = readtable(FilePath_c,'DecimalSeparator', '.','NumHeaderLines', 0);
data_c=dataTab_c{:,:};
headers_c=["t", "\delta_e [deg]","\delta_a [deg]","\delta_r [deg]","Manetta [%]"];

FilePath_sv=strcat(pwd,"\Output\simOutput2.txt");
dataTab_sv = readtable(FilePath_sv,'DecimalSeparator', '.','NumHeaderLines', 0);
data_sv=dataTab_sv{:,:};
headers_sv=["t", "u [m/s]", "v [m/s]", "w [m/s]", "p [rad/s]", "q [rad/s]", "r [rad/s]", "\phi [deg]", "\theta [deg]", "\psi [deg]", "h [m]", "x [m]", "y [m]", "m [kg]"];

%% destinazione figure

% Ottieni il percorso completo della cartella in cui si trova il file .m
cartella_corrente = pwd;

% Crea la sottocartella 'figure' nella cartella corrente
sottocartella = 'Output_post-processing';
cartella_destinazione = fullfile(cartella_corrente, sottocartella);
mkdir(cartella_destinazione);

%% Plot commands
data_c(:,5)=100*data_c(:,5); %Adjust throttle percentage

figure(1)
for i=2:length(data_c(1,:))
    subplot(4,1,i-1);
    plot(data_c(:,1), data_c(:,i), 'b', LineWidth=2);
    grid on
    ylabel(headers_c(i), 'fontname', 'Times New Roman', 'FontSize', 14);
    xlabel("Time[s]", 'fontname', 'Times New Roman', 'FontSize', 14);
    sgtitle('Andamenti nel tempo di \delta_e, \delta_a, \delta_r, \pi', 'fontname', 'Times New Roman', 'FontSize', 16);
end

saveas(gcf, fullfile(cartella_destinazione, 'figure_comandi.png'));
saveas(gcf, fullfile(cartella_destinazione, 'figure_comandi.fig'));
close all

%% Plot all state variables
figure(2)
for i=2:4
    subplot (3,1,i-1)
    plot(data_sv(:,1), data_sv(:,i),'b',LineWidth=2);
    grid on
    ylabel(headers_sv(i), 'fontname', 'Times New Roman', 'FontSize', 14)
    xlabel("Time[s]", 'fontname', 'Times New Roman', 'FontSize', 14);
    sgtitle ('Andamenti nel tempo di u, v, w', 'fontname', 'Times New Roman', 'FontSize', 16) 
end
saveas(gcf, fullfile(cartella_destinazione, 'figure_uvw.png'));
saveas(gcf, fullfile(cartella_destinazione, 'figure_uvw.fig'));
close all

figure(3)
for i=5:7
    subplot (3,1,i-4)
    plot(data_sv(:,1), data_sv(:,i),'b',LineWidth=2);
    grid on
    ylabel(headers_sv(i), 'fontname', 'Times New Roman', 'FontSize', 14)
    xlabel("Time[s]", 'fontname', 'Times New Roman', 'FontSize', 14);
    sgtitle ('Andamenti nel tempo di p, q, r', 'fontname', 'Times New Roman', 'FontSize', 16) 
end
saveas(gcf, fullfile(cartella_destinazione, 'figure_pqr.png'));
saveas(gcf, fullfile(cartella_destinazione, 'figure_pqr.fig'));
close all

figure(4)
for i=8:10
    subplot (3,1,i-7)
    plot(data_sv(:,1), data_sv(:,i),'b',LineWidth=2);
    grid on
    ylabel(headers_sv(i), 'fontname', 'Times New Roman', 'FontSize', 14)
    xlabel("Time[s]", 'fontname', 'Times New Roman', 'FontSize', 14);
    sgtitle ('Andamenti nel tempo di \phi, \theta, \psi', 'fontname', 'Times New Roman', 'FontSize', 16) 
end
saveas(gcf, fullfile(cartella_destinazione, 'figure_angles.fig'));
saveas(gcf, fullfile(cartella_destinazione, 'figure_angles.png'));
close all
%%
figure(5)
plot(data_sv(:,1), data_sv(:,11),'b',LineWidth=2)
grid on
ylabel(headers_sv(11), 'fontname', 'Times New Roman', 'FontSize', 14)
xlabel("Time[s]", 'fontname', 'Times New Roman', 'FontSize', 14);
sgtitle ('Andamento nel tempo della quota', 'fontname', 'Times New Roman', 'FontSize', 16) 
saveas(gcf, fullfile(cartella_destinazione, 'figure_quota.png'));
saveas(gcf, fullfile(cartella_destinazione, 'figure_quota.fig'));
close all
%%
figure(6)
for i=11:13
    subplot (3,1,i-10)
    plot(data_sv(:,1), data_sv(:,i),'b',LineWidth=2);
    grid on
    ylabel(headers_sv(i), 'fontname', 'Times New Roman', 'FontSize', 14)
    xlabel("Time[s]", 'fontname', 'Times New Roman', 'FontSize', 14)
    sgtitle ('Andamenti nel tempo di quota, x, y', 'fontname', 'Times New Roman', 'FontSize', 16) 
end
saveas(gcf, fullfile(cartella_destinazione, 'figure_NED.png'));
saveas(gcf, fullfile(cartella_destinazione, 'figure_NED.fig'));
close all

figure(7)
plot(data_sv(:,1), data_sv(:,14),'b',LineWidth=2)
grid on
ylabel(headers_sv(14), 'fontname', 'Times New Roman', 'FontSize', 14)
xlabel("Time[s]", 'fontname', 'Times New Roman', 'FontSize', 14)
sgtitle ('Andamento nel tempo della massa', 'fontname', 'Times New Roman', 'FontSize', 16) 
saveas(gcf, fullfile(cartella_destinazione, 'massa.png'));
saveas(gcf, fullfile(cartella_destinazione, 'massa.fig'));
close all


%% Plot velocità V
figure(8)
V=sqrt(data_sv(:,2).^2 + data_sv(:,3).^2 + data_sv(:,4).^2);
V_stall=30*V./V;
V_max=75*V./V;
title('Andamento della velocità nel tempo' ,'fontname','Times New Roman','FontSize',16)
hold on
plot(data_sv(:,1), V_stall(1:end),'y','LineWidth',2)
hold on
plot(data_sv(:,1), V_max(1:end),'r','LineWidth',2)
hold on
plot(data_sv(:,1), V(1:end),'b','LineWidth',2)
grid on
xlabel('Tempo[s]' ,'fontname','Times New Roman','FontSize',14,'color',[0,0,0])
ylabel('Velocità V[m/s]' ,'fontname','Times New Roman','FontSize',14,'color',[0,0,0])
title('Andamento della velocità nel tempo' ,'fontname','Times New Roman','FontSize',16)
legend ('V di stallo','V_{NE}', 'V')
axis([data_sv(1,1) data_sv(end,1) 25 80])
saveas(gcf, fullfile(cartella_destinazione, 'figure_velocità.png'));
saveas(gcf, fullfile(cartella_destinazione, 'figure_velocità.fig'));
close all


%% Plot alpha e beta
figure(9);
alpha=(atan2(data_sv(:, 4), data_sv(:, 2))).*(180/pi);
beta=(atan2(data_sv(:, 3), data_sv(:, 2)))*(180/pi);
subplot(2,1,1)
plot(data_sv(:,1), alpha, 'b', LineWidth=2);
grid on
ylabel("\alpha [deg]")
title("Andamento nel tempo di \alpha", 'fontname', 'Times New Roman', 'FontSize', 16)
subplot(2,1,2)
plot(data_sv(:,1), beta, 'r', LineWidth=2);
grid on
xlabel("Time [s]")
ylabel("\beta [deg]")
title("Andamento nel tempo di \beta", 'fontname', 'Times New Roman', 'FontSize', 16)
saveas(gcf, fullfile(cartella_destinazione, 'figure_alfabeta.png'));
saveas(gcf, fullfile(cartella_destinazione, 'figure_alfabeta.fig'));
close all


%% Plot traiettoria 3D
figure(10)
plot3(data_sv(:, 12), data_sv(:, 13), data_sv(:, 11), 'b', LineWidth=2)
hold on
plot3(data_sv(1, 12), data_sv(1, 13), data_sv(1, 11), 'go', LineWidth=2);
hold on
plot3(data_sv(end, 12), data_sv(end, 13), data_sv(end, 11), 'ro', LineWidth=2);
grid on
title("Traiettoria 3D", 'fontname', 'Times New Roman', 'FontSize', 16)
xlabel("x_{NED} [m]", 'fontname', 'Times New Roman', 'FontSize', 14)
ylabel("y_{NED} [m]", 'fontname', 'Times New Roman', 'FontSize', 14)
zlabel("h [m]", 'fontname', 'Times New Roman', 'FontSize', 14)
legend('Traiettoria', 'Partenza', 'Arrivo')
% zlim([min(data_sv(:,11))-1000 max(data_sv(:,11))+1000])
zlim([0 4116])
set(gca, 'YDir','reverse')
saveas(gcf, fullfile(cartella_destinazione, 'figure_traiettoria3D.png'));
saveas(gcf, fullfile(cartella_destinazione, 'figure_traiettoria3D.fig'));
close all


%% Plot traiettoria 2D
figure(11)
plot(data_sv(:, 12), data_sv(:, 13), 'bl','LineWidth',2)
hold on
plot(data_sv(1, 12), data_sv(1, 13), 'go', LineWidth=2);
hold on
plot(data_sv(end, 12), data_sv(end, 13), 'ro', LineWidth=2);
title('Traiettoria 2D' ,'fontname','Times New Roman','FontSize',16)
xlabel('x_{NED} [m]' ,'fontname','Times New Roman','FontSize',14)
ylabel('y_{NED}[m]' ,'fontname','Times New Roman','FontSize',14)
legend('Traiettoria', 'Partenza', 'Arrivo');
set(gca, 'YDir','reverse')
hold off
grid on
saveas(gcf, fullfile(cartella_destinazione, 'figure_traiettoria2D.png'));
saveas(gcf, fullfile(cartella_destinazione, 'figure_traiettoria2D.fig'));
close all


%% Andamento spinta e quota nel tempo
figure(12)
plot(data_sv(:,1), data_sv(:,15),'b',LineWidth=2)
grid on
ylabel('Thrust [N]', 'fontname', 'Times New Roman', 'FontSize', 14)
xlabel("Time[s]", 'fontname', 'Times New Roman', 'FontSize', 14)
sgtitle ('Andamento nel tempo della spinta', 'fontname', 'Times New Roman', 'FontSize', 16) 

saveas(gcf, fullfile(cartella_destinazione, 'figure_spinta.png'));
saveas(gcf, fullfile(cartella_destinazione, 'figure_spinta.fig'));
close all




















