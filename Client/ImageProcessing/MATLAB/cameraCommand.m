function controlResponse = cameraCommand(value,key_str)
    %This function changes a camera setting then it returns
    %the value stored in that setting buffer
    %If the value was changed succesfully, it will return the same value as
    %defined by 'value'
    val_str = num2str(value);
    controlURL = strcat('http://192.168.0.160/control?var=',key_str,'&val=',val_str);
    webread(controlURL);
    controlResponse = cameraStatus().(key_str); %reads the value back from the sensor
end

