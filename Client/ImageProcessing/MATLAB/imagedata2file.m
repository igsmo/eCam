%This program turns a stream of image data into an image file.
%single color channel is assumed (grayscale)
clear all;
                        %datafile to open      uint8 for most images
image_string = readmatrix('image.csv','OutputType','uint8','Delimiter',{',',' '}); 
                                                            %Delimiter can be either comma or space

%image dimensions 
image_width = 96;
image_height = 96;
img_2D = zeros(image_width,image_height,"uint8"); %create empty matrix for 2Dimage

%check if dimensions match data
if image_height*image_width ~= length(image_string)
    disp('Error: Image dimensions do not match data')
    return
end

data_index = 1;
%begin reordering data to a 2-dimensional array
for y = 1:image_height
    for x = 1:image_width
        img_2D(x,y)=image_string(data_index);
        data_index = data_index+1;
    end
end

imwrite(img_2D,'resulting.png'); %make a file

imshow('resulting.png'); %display the image file