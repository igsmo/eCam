% This script sends commands to the esp32 camera via http and automates 
% updating camera settings and grabbing frames for comparing the results
% of changing the settings  (this script is work-in-progress)
pause('on');
clear cam;

c_exposure = 'brightness';

cam = ipcam('http://192.168.0.160:81/stream');
pause(1);
preview(cam);
pause(4);


%Collect images for each brightness level
for v_brightness=-2:2
    resp = cameraCommand(v_brightness,c_brightness);
    pause(2);
    notify = strcat('exposure = ',string(resp));
    disp(notify)
    image_snapshot = snapshot(cam);
    file_name = strcat('brightness_',num2str(v_brightness),'_ecam.png');
    imwrite(image_snapshot,file_name,'png');
end

cameraCommand(0,c_brightness); %back to default value

%Display a montage of saved images
%dirOutput = dir(fullfile("brightness_*.png"));
%fileNames = string({dirOutput.name});
%montage(fileNames,"Size",[1 5]);
%title("brightness")

img{1} = imread('brightness_-2_ecam.png');
img{2} = imread('brightness_-1_ecam.png');
img{3} = imread('brightness_0_ecam.png');
img{4} = imread('brightness_1_ecam.png');
img{5} = imread('brightness_2_ecam.png');

fig = figure(); 
tlo = tiledlayout(fig,1,5,'TileSpacing','None');
for i = 1:numel(img)
    ax = nexttile(tlo); 
    imshow(img{i},'Parent',ax)
    title(['Brightness ', num2str(i-3)])
end