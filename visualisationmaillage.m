clear all ;

%% Charger les donnees depuis les fichiers

%data = dlmread('C:\Users\mayeu\Desktop\2A ENSTA x Master IPP\SIM202\GitSIM202\SIM202\maillage_regulier.txt') ;           % Cas non régulier
%data = dlmread('C:\Users\mayeu\Desktop\2A ENSTA x Master IPP\SIM202\GitSIM202\SIM202\maillage_nonregulier.txt') ;        % Cas non régulier
%data = dlmread('C:\Users\mayeu\Desktop\2A ENSTA x Master IPP\SIM202\GitSIM202\SIM202\maillage_secteurangulaire.txt') ;   % Cas non régulier
data = dlmread('C:\Users\mayeu\Desktop\2A ENSTA x Master IPP\SIM202\GitSIM202\SIM202\maillage_front.txt')

% Extraire les sommets et les triangles
sommets = data(data(:, 1) == 0, 2:3) ;
triangles = data(data(:, 1) == 1, 2:4 ) ;

%% Visualiser le maillage

triplot(triangles(:, [1 2 3]), sommets(:, 1), sommets(:, 2), 'b') ;

hold on ;
scatter(sommets(:, 1), sommets(:, 2), 'r') ;
xlabel('X') ;
h = ylabel('Y') ;
set(h, 'Rotation', 0) ; % Rotation du label de l'axe des ordonnées
title('Maillage') ;
axis equal ;
axis tight;

%margin = max(sommets(:))/7 ; % Ajoutez la marge souhaitée
%axis([-margin, max(sommets(:, 1)) + margin, -margin, max(sommets(:, 2)) + margin]) ;

grid on ;
grid on ;
hold off ;
