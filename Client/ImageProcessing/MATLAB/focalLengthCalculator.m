%Script to calculate focal length

%Sensor parameters according to datasheet for OV2640
s_hmm = 2.684;%sensor height in mm
s_hpx = 1200; %sensor height in number of pixels
s_wmm = 3.590;%sensor width in mm  3590 µm x 2684 µm
s_wpx = 1600; %sensor width in number of pixels

%Actual object parameters in test setup
o_zs_real = 18.1; %real object size in mm
dist = 200; %distance from camera to object in mm
%parameter measured from captured image of test setup
o_hpx = 143; %object height in number of pixels
%f_l is the focal length of the camera
f_l = dist*s_hmm*o_hpx/(o_zs_real*s_hpx);

%CALCULATIONS FOR QR CODE SIZE IN SMALL FACTORY
%avarage calculated focal length of OV2640 from 6 images:
f_avg = 3.542768784530387;
dist_max = 4000; %maximum distance to QR-code in the factory (mm)
o_hpix_min = 4; %minimum amount of pixels in the captured image to represent object
% Calculate minumum "pixel size" of QR code at distance of 4m
QRpixmin = s_hmm*o_hpix_min*dist_max/(s_hpx*f_avg);

%CALCULATIONS FOR PIXEL MOVEMENT DRIVING STRAIGHT TOWARDS TARGET
opw1 = 8; %Objects percieved width @Distance 1 (use pixels as input to get pixels as output)
D = (4000:-10:0); %Distance 1, or reference distance
pmx = zeros(size(D2,2),1);
pmx_sum = zeros(size(D2,2)+1,1);
for i = 2:(size(D2,2))
    pmx(i,1)=(opw1*D(1,i-1))/(2*D(1,i)); %perceived movement of point along x-axis
    pmx_sum(i,1)=pmx(i,1)+pmx_sum(i-1,1);
end

plot(pmx_sum)

% CALCULATIONS FOR MAX EXPOSURE TIME
% B = 4; %Maximum blurred pixels (in direction of movement)
% FOV = %Field of View length (in axis of movement @4m distance)
% Vo = %Velocity of object
% Np = %Number of pixels spanning FOV (either horizontal or vertical)
% Te = B*FOV/(Vo*Np); %Max exposure time