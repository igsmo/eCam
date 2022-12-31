function loadSettings(filename)
% This program loads camera settings from a csv file
pause('on');

%read settings from file
camera_settings = readcell(filename); 

for i = 1:(size(camera_settings,1))
    key_str = cell2mat(camera_settings(i,1));value = cell2mat(camera_settings(i,2));resp = value+1;
    while (resp ~= value)
        resp = cameraCommand(value,key_str); %change setting
        if (resp ~= value) %check response
            pause(1); %Pause before trying again
        end
    end
end
%The program will only reach the end if all settings were changed
%succesfully. Else it will be stuck in the while loop forever.
end

