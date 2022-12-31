%Script to estimate motion blur from moving pixels in an application
%where the camera is moving towards a stationary target (or vise versa)

%Sensor parameters according to datasheet for OV2640
s_hmm = 2.684;%sensor height in mm
s_hpx = 1200; %sensor height in number of pixels
s_wmm = 3.590;%sensor width in mm  3590 µm x 2684 µm
s_wpx = 1600; %sensor width in number of pixels

f_avg = 3.542768784530387; %measured focal length of lens
QRpixmin = 12; %actual size of QR-code "pixel" determined in earlier step (mm)
QRwidth = QRpixmin * 21; %width of entire QR code (mm)
o_pixw_min = QRwidth*s_wpx*f_avg/(4000*s_wmm); %minimum pixel width of object (at max distance)
o_ompc_min = o_pixw_min/2; %Coordinate of the outer most sensor pixel along the x-axis at maximum distance

v_speed = 750; %Speed of vehicle (mm pr. second)
fps = 15; %framerate of camera
exp_div = 8; %division factor for exposure time
dpe = (v_speed*((1/fps))); %distance per exposure period (mm)
%% 

%CALCULATIONS FOR PIXEL MOVEMENT DRIVING STRAIGHT TOWARDS TARGET

D = (4000:-dpe:dpe); %Distances from max distance at the interval of distance per exposure time
delta_pixel = zeros(size(D,2),1);
o_ompc = zeros(size(D,2),1);
o_hpix = zeros(size(D,2),1);
for i = (1:1:size(D,2))
    o_ompc(i,1) = (QRwidth*s_wpx*f_avg/(D(1,i)*s_wmm))/2; %outer most pixel coordinate of QR code on sensor
    o_hpix(i,1) = (QRpixmin*s_hpx*f_avg)/(s_hmm*D(1,i)); %size of digit in sensor pixels
    if (i == 1)
        delta_pixel(i,1) = 0;
    else
        delta_pixel(i,1) = o_ompc(i,1)-o_ompc(i-1,1);
    end
end

yyaxis left
%plot(D,o_ompc) %plot displacement
plot(D,o_hpix)  %plot digit size in pixels
set(gca, 'XDir','reverse')
xlabel('Distance to target') 
%ylabel('x-coordinate of outermost point')  %label for displacement
ylabel('Size of QR digit in sensor pixels')   %label for digit size
%axis([0 4000 0 2000]);
axis([0 4000 0 40]);
yyaxis right
plot(D,delta_pixel)
axis([0 4000 0 40]);
ylabel('Pixel displacement within exposure period') 


% CALCULATIONS FOR MAX EXPOSURE TIME
% B = 4; %Maximum blurred pixels (in direction of movement)
% FOV = %Field of View length (in axis of movement @4m distance)
% Vo = %Velocity of object
% Np = %Number of pixels spanning FOV (either horizontal or vertical)
% Te = B*FOV/(Vo*Np); %Max exposure time