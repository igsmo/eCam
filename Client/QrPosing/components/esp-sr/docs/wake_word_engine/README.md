# WakeNet  

WakeNet, which is a wake word engine built upon neural network, is specially designed for low-power embedded MCUs. Now, the WakeNet model supports up to 5 wake words.

## Overview

Please see the flow diagram of WakeNet below:

<center>
<img src="../img/wakenet_workflow.png" width = "800" />
</center>

- Speech Feature:  
  The WakeNet uses [MFCC](https://en.wikipedia.org/wiki/Mel-frequency_cepstrum) to obtain the features of the input audio clip (16 KHz, 16 bit, single track). The window width and step width of each frame of the audio clip are both 30 ms. 
      
- Neural Network:  
  Now, the neural network structure has been updated to the sixth edition, among which,  
  - WakeNet1 and WakeNet2 had been out of use.
  - WakeNet3 and WakeNet4 had been out of use.
  - WakeNet5(WakeNet5X2,WakeNetX3), WakeNet7, WakeNet8 are built upon the [Dilated Convolution](https://arxiv.org/pdf/1609.03499.pdf) structure. 
  Note that，The network structure of WakeNet5,WakeNet5X2 and WakeNet5X3 is same, but the parameter of WakeNetX2 and WakeNetX3 is more than WakeNet5. Please refer to [Resource Occupancy](#performance-test) for details.

         
- Keyword Triggering Method  
  For continuous audio stream, we calculate the average recognition results (M) for several frames and generate a smoothing prediction result, to improve the accuracy of keyword triggering. Only when the M value is larger than the set threshold, a triggering command is sent.  

The following table shows the model support of Espressif SoCs:

![wakent_model](../img/WakeNet_model.png)

## API Introduction

- How to select the WakeNet model  
  
  1. Go to `make menuconfig`, navigate to `Component config` >> `ESP Speech Recognition` >> `Wake word engine`. See below:  
  
     <center> <img src="../img/model_sel.png" width = "500" /> </center>    
     
  2. WakeNet6 is divided into two tasks， task1 is used to calculate speech recognition， the task2 is used to calculate neural network model. The ESP32 core used to calculate task2 can be selected by `Component config` >> `ESP Speech Recognition` >> `ESP32 core to run WakeNet6`    

  
  

- How to select the wake words   
  Go to `make menuconfig`, and navigate to `Component config` >> `ESP Speech Recognition` >> `Wake words list`. See below:  

  <center>
  <img src="../img/word_sel.png" width = "500" />
  </center>

  Note that, the `customized word` option only supports WakeNet5 and WakeNet6. WakeNet3 and WakeNet4 are only compatible with earlier versions. If you want to use your own wake words, please overwrite existing models in `wake_word_engine` directory with your own words model.

- How to set the triggering threshold  
  1. The triggering threshold (0, 0.9999) for wake word can be set to adjust the accuracy of the wake words model. The threshold can be configured separately for each wake words if there are more than one words supported in a model. 
  2. The smaller the triggering threshold is, the higher the risk of false triggering is (and vice versa). Please configure your threshold according to your applications.   
  3. The wake word engine predefines two thresholds for each wake word during the initialization. See below:
  
    ```
    typedef enum {
	    DET_MODE_90 = 0,      // Normal, 
	    DET_MODE_95 = 1,      // Aggressive, 
	    DET_MODE_2CH_90 = 2,  // 2 Channel detection, Normal mode 
	    DET_MODE_2CH_95 = 3,  // 2 Channel detection, Aggressive mode
	    DET_MODE_2CH_90 = 4,  // 3 Channel detection, Normal mode 
	    DET_MODE_2CH_95 = 5,  // 3 Channel detection, Aggressive mode
    } det_mode_t;
    ```
    
  4. Use the `set_det_threshold()` function to configure the thresholds for different wake words after the initialization.
  
- How to get the sampling rate and frame size.
  - Use `get_samp_rate` to get the sampling rate of the audio stream to be recognized.
  - Use `get_samp_chunksize` to get the sampling point of each frame. The encoding of audio data is `signed 16-bit int`.
  
## Performance Test

### 1. Resource Occupancy(ESP32)

|Model Type|Parameter Num|RAM|Average Running Time per Frame| Frame Length|
|:---:|:---:|:---:|:---:|:---:|
|Quantised WakeNet5|41 K|15 KB|5.5 ms|30 ms|
|Quantised WakeNet5X2|165 K|20 KB|10.5 ms|30 ms|
|Quantised WakeNet5X3|371 K|24 KB|18 ms|30 ms|

### 2. Resource Occupancy(ESP32S3)
|Model Type|Parameter Num|RAM|Average Running Time per Frame| Frame Length|
|:---:|:---:|:---:|:---:|:---:|
|Quantised WakeNet7_2CH|810 K|45 KB|10 ms|32 ms|
|Quantised WakeNet8_2CH|821 K|50 KB|10 ms|32 ms|


### 2. Performance

|Distance| Quiet | Stationary Noise (SNR = 4 dB)| Speech Noise (SNR = 4 dB)| AEC Interruption (-10 dB)|
|:---:|:---:|:---:|:---:|:---:|
|1 m|98%|96%|94%|96%|
|3 m|98%|94%|92%|94%|

False triggering rate: 1 time in 12 hours

**Note**: We use the ESP32-S3-Korvo V4.0 development board and the WakeNet8(Alexa) model in our test. 

## Wake Word Customization

For details on how to customize your wake words, please see [Espressif Speech Wake Word Customization Process](ESP_Wake_Words_Customization.md).
