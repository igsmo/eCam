%IQA: image quality assesment of image

cd rawrez\
imagefiles = dir('*.png');
nfiles = length(imagefiles);    % Number of files found
IQAscores = table('Size',[nfiles 7],'VariableTypes',{'string','double','double','double','double','double','double'},'VariableNames',["image","Brisque","Niqe","Piqe","Sharpness","Entropy","Noise"]);


for ii=1:nfiles
   IQAscores.image(ii) = imagefiles(ii).name;
   currentimage = imread(imagefiles(ii).name);
   %IQAscores.Brisque(ii) = brisque(currentimage);
   %IQAscores.Niqe(ii) = niqe(currentimage);
   IQAscores.Piqe(ii) = piqe(currentimage);
   if ndims(currentimage) == 3
       grayimage = double(rgb2gray(currentimage));  %convert image to grayscale
       gray_uint8 = rgb2gray(currentimage);
       IQAscores.Sharpness(ii) = estimate_sharpness(grayimage);
       IQAscores.Entropy(ii) = entropy(gray_uint8);
       IQAscores.Noise(ii) = NoiseLevel(grayimage);
   else
       grayimage = double(currentimage);
       IQAscores.Sharpness(ii) = estimate_sharpness(grayimage);
       IQAscores.Entropy(ii) = entropy(currentimage);
       %IQAscores.Noise(ii) = NoiseLevel(currentimage);
   end
end

filename = 'rawrezIQAscores.xlsx';
writetable(IQAscores,filename,'Sheet',1,'Range','A1')


% Estimate sharpness using the gradient magnitude.
% sum of all gradient norms / number of pixels give us the sharpness
% metric.
function [sharpness]=estimate_sharpness(G)

[Gx, Gy]=gradient(G);

S=sqrt(Gx.*Gx+Gy.*Gy);
sharpness=sum(sum(S))./(numel(Gx));

end

function Sigma=estimate_noise(I)
[H W]=size(I);
I=double(I);
% compute sum of absolute values of Laplacian
M=[1 -2 1; -2 4 -2; 1 -2 1];
Sigma=sum(sum(abs(conv2(I, M))));
% scale sigma with proposed coefficients
Sigma=Sigma*sqrt(0.5*pi)./(6*(W-2)*(H-2));
end