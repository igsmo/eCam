%Takes a RAW image and resizes it by a moving window avarage.

image = imread("2022121381745_eCam_RAW.png");

x_ori = 1600;
y_ori = 1200;

x_out = 96;
y_out = 96;

data = zeros([y_out,x_out],'uint8');
pix_data = uint32(0); %placeholder to collect data from pixels

x_wind = fix(x_ori/x_out);  %This is the size of the window
y_wind = fix(y_ori/y_out);
p_sz = x_wind*y_wind; %number of input pixels for output pixel

x_rem = mod(x_ori,x_out);   %remainder. These pixels are skipped.
y_rem = mod(y_ori,y_out);



for yw = 1:y_out   %move window y-axis
    for xw = 1:x_out   %move window x-axis
        pix_data = 0;
        for y = 1:y_wind %copy line
            for x = 1:x_wind %collect pixel
                pix_data = pix_data + uint32(image(((yw-1)*y_wind)+y_rem+y,((xw-1)*x_wind)+x));
            end
        end
        %avarage out the pix_data and store it to the data array
        data(yw,xw) = pix_data / p_sz;
    end
end    

imshow(data);