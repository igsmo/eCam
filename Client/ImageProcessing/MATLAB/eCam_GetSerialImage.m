clear all; clc; clear s;

pause ('on');

height = 1200; %set image height and width here
width = 1600;
data = zeros([height,width],'uint8'); %array for image

ports = serialportlist %prints available serial ports
device = "COM4";
s=serialport(device,115200,"Timeout",10);  %Note: port starts buffering incoming data immedietly
pause(0.2); %wait for serial to start

write(s,"1","char"); %This signals to the ESP that it can transmit the data
pause(3); %wait a bit for transmission to start

transm = 0;
%this routine clears any unwanted data from the serial recieve buffer that
%might be placed before the image data begins. And detects the 'start'
%signal that occurs just before the image data is sent.
while transm == 0
    recieved = read(s,1,"uint8");
    if recieved == 0
        recieved = read(s,1,"uint8");
        if recieved == 255
            recieved = read(s,1,"uint8");
                if recieved == 0
                    recieved = read(s,1,"uint8");
                        if recieved == 255
                            transm = 1; %'start' signal has been detected
                        end
                end
        end
    end
end %end while  

disp("Start signal detected. Storing image data. This will take close to 3 minutes.")
%START STORING IMAGE DATA
    for h_ind = 1:height
            for w_ind = 1:width
                data(h_ind,w_ind) =  read(s,1,"uint8");
            end %end for width
        disp(h_ind)
    end %end for height


    %CREATE IMAGE FILE
            %unique timestamp
            time_end = fix(clock);
            timestamp = strcat(num2str(time_end(1,1)),num2str(time_end(1,2)),num2str(time_end(1,3)),num2str(time_end(1,4)),num2str(time_end(1,5)),num2str(time_end(1,6)));
            filename = strcat(timestamp,'_eCam_RAW.png')
            %write file
            imwrite(data,filename,'png');

            clear s;
