function status = cameraStatus()
    %This function reads the settings from the camera sensor
    %This is not a complete list of settings
    %Only those defined by the http server app
    statusURL = 'http://192.168.0.160/status';
    status = webread(statusURL);
    %disp(status)
end

