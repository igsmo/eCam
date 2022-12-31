% This script simulates motion blurr radiating from the centre of the image
% As would happen when the camera moves directly towards the target
%Note: the program assumes an image of aspect ratio 3:2
cd 40cm\
%first load an image from file
image = imread("500_LUX_40cm_defaultsettings_ecam.png");
x_siz = 1600;
y_siz = 1200;
%Rezise the image by 3 pixels horizontally and 2 pixels vertically
% As the aspect ratio of the image is 3:2
%B = imresize(A,[numrows numcols])
pixel_blur_y = 4; %should be divisible by 2 and multiple of 3. So: 6, 12, 18, 24..
pixel_blur_x = pixel_blur_y*(2/3);  %2/3 for aspect ratio of 3:2

for i = 1:pixel_blur_y*10
    if (i == 1)
        resized = imresize(image,[(y_siz+(i*0.1)) (x_siz+(i*0.1)*(2/3))]);
        cropped = imcrop(resized,[((i*0.1)/2) (((i*0.1)*(2/3))/2) (x_siz-1) (y_siz-1)]);
        fused(:,:,:,i) = imfuse(image,cropped,'blend','Scaling','joint');
    else
        resized = imresize(fused(:,:,:,i-1),[(y_siz+(i*0.1)) (x_siz+(i*0.1)*(2/3))]);
        cropped = imcrop(resized,[((i*0.1)/2) (((i*0.1)*(2/3))/2) (x_siz-1) (y_siz-1)]);
        fused(:,:,:,i) = imfuse(fused(:,:,:,i-1),cropped,'blend','Scaling','joint');
    end
end

f1 = figure; f2 = figure;
imshow(fused(:,:,:,i));
figure(f1);
imshow(image);