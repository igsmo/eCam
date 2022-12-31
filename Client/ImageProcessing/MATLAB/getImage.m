function [file_name] = getImage(input_name)
%This function takes a snapshot and names the file according to the input
%LUX level and distance to the QR-code

    if exist('cam','var')
        clear cam;
    end

    cam = ipcam('http://192.168.0.160:81/stream','Timeout',60); %Home IP
    %cam = ipcam('http://192.168.0.29:81/stream','Timeout',60); %AP IP
    loadSettings('settings_manAEC87.csv');
    
    %When a new camera object is created, it will be at the default values
    %defined in the embedded code
    %Here you can override that with your own settings
    %espOV2640_defaultSettings();    %this loads the default settings

    % take 11 snapshot to be sure that the snapshot saved is a new one and
    % not something old from the buffer.
    for i = 1:2
        image_snapshot = snapshot(cam);
    end
    
    %Then write the lates snapshot to a file
    file_name = strcat(input_name,'_manAEC87.png');
    imwrite(image_snapshot,file_name,'png');
    clear cam;
end

