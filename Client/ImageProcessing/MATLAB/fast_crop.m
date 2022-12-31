%This takes an image and crops it to a desired level

image = imread("2022121381745_eCam_RAW.png");

x_ori = 1600;   %Dimensions of the input image
y_ori = 1200;
image1D = zeros([1,x_ori*y_ori],'uint8');   %Array for the 1D image

%Turn the 2D image to 1D image data. As it would be stored in memory
for y=1:1200
    for x=1:1600
        k = ((y-1)*1600)+x;
        image1D(1,k) = image(y,x);
    end
end

x_out = 400;    %Dimensions of output image
y_out = 300;
data = zeros([y_out,x_out],'uint8');    %2D array
data1D = zeros([1,x_out*y_out],'uint8');    %1D array

y_offs = (y_ori-y_out)/2;   %Offset needs only be calculated once 
x_offs = (x_ori-x_out)/2;   % and could be stored as a constant

%Do a cropping of 1D image and output a smaller 1D image.
for y=1:300
    for x=1:400
        k = ((y-1)*400)+x;
        i = ((y-1+y_offs)*1600)+x+x_offs;
        data1D(1,k) = image1D(1,i);
    end
end

%Turn the 1D cropped image back into a 2D array
% This is just to see that it looks OK.
for y=1:300
    for x=1:400
        data(y,x) = data1D(1,((y-1)*400)+x);
    end
end

%regular 2D to 2D cropping
% for y=1:300
%     for x=1:400
%         data(y,x) = image(y_offs+y,x_offs+x);
%     end
% end

imshow(data);