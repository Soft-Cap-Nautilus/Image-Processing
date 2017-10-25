% Author: Henrique José dos Santos Ferreira Júnior
% This code was developed to run into UFRJ-Nautilus' AUV, UFRJ-Nautilus is
% the first brazilian team to participate of RoboSub Competition.
% Please quote us if use this code.

% A ideia por trás do código é filtrar a guia pela sua cor característica
% para através da imagem suficientemente binarizada achar as quinas da guia
% que são 4 pixeis de valor 1, um com maior coordenada X, outro menor X,
% outro maior Y e outro menor Y. O melhor dos casos para a detecção desses
% pontos é quando a fita está rotacionada de 45º, pois esses pontos serão
% únicos. Pior dos casos é 0º e 90º.


RED_MIN_FILTER = 200; % Set the red lower value
GREEN_MIN_FILTER = 240; % Set the green lower value
BLUE_MAX_FILTER = 255; % Set the blue maximum value

im = imread('guide2.png'); % Open the image
[linha, coluna, canais] = size(im); % Get Image Size
imB = zeros(linha, coluna); % Start a binary image with all zeros

disp(' '); % New Line

maxL = 0; % Aux variable to take the pixel with value 1 which have the highest Y value.
minL = linha; % Aux variable to take the pixel with value 1 which have the lowest Y value. 
maxC = 0; % Aux variable to take the pixel with value 1 which have the highest X value.
minC = coluna; % Aux variable to take the pixel with value 1 which have the lowest X value.

for L = 1:linha % For each line in the image
    for C = 1:coluna % For each pixel in the line
        if im(L,C,1) > RED_MIN_FILTER && im(L,C,2) > GREEN_MIN_FILTER && im(L,C,3) < BLUE_MAX_FILTER % Filter the Red, Green and Blue values
            imB(L,C) = 1; % Set it pixel with value 1 on our binary image
            if L>maxL % Find the pixel with value 1 which have the highest Y value.
                maxL = L;
                cmaxL = [L,C]; % Take it coordinate
            end
            if L<=minL % Find the pixel with value 1 which have the lowest Y value. 
                minL = L;
                cminL = [L,C]; % Take it coordinate
            end
            if C>=maxC % Find the pixel with value 1 which have the highest X value.
                maxC = C;
                cmaxC = [L,C]; % Take it coordinate
            end
            if C<minC % Find the pixel with value 1 which have the lowest X value.
                minC = C;
                cminC = [L,C]; % Take it coordinate
            end
        end
    end
end


           