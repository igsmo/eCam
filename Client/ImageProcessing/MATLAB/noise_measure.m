%measure the level of noise in a single image from a folder of images

cd 40cm\
imagefiles = dir('*.png');
nfiles = length(imagefiles);    % Number of files found
IQAscores = table('Size',[nfiles 2],'VariableTypes',{'string','double'},'VariableNames',["image","Noise"]);


for ii=1:nfiles
   IQAscores.image(ii) = imagefiles(ii).name;
   currentimage = imread(imagefiles(ii).name);
   if ndims(currentimage) == 3
       grayimage = double(rgb2gray(currentimage));  %convert image to grayscale
       IQAscores.Noise(ii) = NoiseLevel(grayimage);
   else
        IQAscores.Noise(ii) = NoiseLevel(currentimage);
   end
   
end

filename = '40cmNoise_scores.xlsx';
writetable(IQAscores,filename,'Sheet',1,'Range','A1')