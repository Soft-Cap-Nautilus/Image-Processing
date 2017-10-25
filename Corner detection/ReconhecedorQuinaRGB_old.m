im = imread('guide2.png');
redFILTER = (im(1:1:end, 1:1:end, 1)>200);
greenFILTER = (im(1:1:end, 1:1:end, 2)>240);
imB = redFILTER .* greenFILTER;
[linha, coluna] = size(imB);
disp('novo');
maxL = 0;
minL = linha;
maxC = 0;
minC = coluna;
for L = 1:linha
    for C = 1:coluna
        if imB(L,C)==1
            if L>maxL
                maxL = L;
                cmaxL = [L,C];
                
            end
            if L<=minL
                minL = L;
                cminL = [L,C];
                
            end
            if C>=maxC
                maxC = C;
                cmaxC = [L,C];
                
            end
            if C<minC
                minC = C;
                cminC = [L,C];
                
            end
        end
    end
end


           