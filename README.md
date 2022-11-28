# HSE.2DImageProcessing.Week3

Hello my fellow classmates 👋

Thank you for reading this introduction document! Hopefully, I would be able to keep you interested 😅

## Build

To build the project you have to have [OpenCV installed](https://docs.opencv.org/4.x/d7/d9f/tutorial_linux_install.html) on your computer. 

When everything is ready you need to navigate to `src` folder in your terminal and execute the following chain of commands.

```
mkdir build
cd build

cmake ..
make -j7
```

The build will be finished shortly after calling the command and you can expect to see the executable file inside `bin` directory. Hopefully, the file is there and you are ready to move to the next step!

## Command

Generally, the application accepts a few command line arguments. If there was a _help_ command it would output something similar to the text below.

```bash
./bin/CoinDetector image1 image_folder1 image2 ... imageN [-o output prefix] [-c metrics config] [-d debug]
```

However, it can be a bit confusing. Let me breifly explain what these arguments do. Basically, the invocation consists of two important groups: an images list and additional arguments.

1. **Images list**: just a list of either image files or folders with images. If an entry is an images folder then all images are recursively extracted from within it.

2. **Additional arguments**: all arguments are optional. If you decide to use them read thoroughly the infromation in the table below.

|Argument|Desciption|
|-----|-----|
|-d| *Debug flag*: if specified then the application displays additional processing steps, otherwise only result image with detected circles will be shown. |
|-o| *Output files prefix*: specifies the final directory for output images. If not specified then the final images will be saved in the application folder, ie the folder from where the application is located. |
|-c| *Config metrics file*: if specified then the passed images will be tested against this config and prediction metrics will be calculated. Config should contain information about coins in the given images. To create a custom config see [Config section](Config). |

### Usage example

To give you a better context on how to use the application I provided a few usage examples.

- Folder with images + metrics config

```base
./bin/CoinsDetector ../../Samples -c ../../Samples/circles.txt
```

- Single image + metrics config

```bash
./bin/CoinsDetector ../../Samples/20.jpg -c ../../Samples/circles.txt
```

- Single image + debug flag

```bash
./bin/CoinsDetector ../../Samples/20.jpg -d
```

- Single image + metrics config + custom output prefix

```bash
./bin/CoinsDetector ../../Samples/19.jpg -c ../../Samples/circles.txt -o ../../Samples
```

## Config

Config describes the given images set. When the images set is tested against config all images from the set should be on the config, however, the config may have more images descriptors than the set has. If the image is not found on the set [runtime_error](https://en.cppreference.com/w/cpp/error/runtime_error) is rised.

To make your own config or understand better how the original config is composed you may find these set of rules quite useful:

- Every image that you want to test should be described in the config;
- Every image has its own descriptor/config section;
- There should be an empty line between sections;
- A section should distinctly identify the image using image id followed by a circles list;
- *image id* is __image name__ + __image extension__, for examples, for the field `./a/b/image.png` the id is `image.png`;
- Every circle is a triple of x and y coordinates and a radius.

These rules can be visualised in the following way:

```text
image_id_1
x1,y1,r1
x2,y2,r2
x3,y3,r3

image_id_2
x1,y1,r1

...

image_id_n
x1,y1,r1
x2,y2,r2
xN,yN,rN
```

You can see how these rules are applied looking at [the given config](./Samples/circles.txt):

```text
01.jpg
185,180,66
384,285,98
609,204,111
171,369,84
343,478,58
602,504,104
169,608,81
421,638,62

02.jpg
64,66,65
185,55,43
310,62,59
56,186,54
186,184,76
313,187,53
51,306,60
142,310,39
225,309,44
320,302,50

...
```

## Test Samples

There are 20 test sample images that form the test set. All coins in the set are are distinguihsable from background, well illuminated, and do not have extreme points views.

To give you a better understanding of the tests' nature you can see the example.

![Example](./Samples/03.jpg)

You may take a look at the rest of the test set by looking at [the samples folder](./Samples/).

## Algorithm

The algorithm used in the project is not that sophisticated and can be represented as a chain of several steps.

*Note: To reproduce the same behaviour run the following command:*

```bash
./bin/CoinsDetector ../../Samples/13.jpg  -c ../../Samples/circles.txt -o ../../Images -d
```

| Step | Image  | Description  |
| ------- | --- | --- |
| 0. Original image | ![Original](./Samples/13.jpg) | Unmodified image. This is not an actual step. I put it here to show the original image. |
| 1. Read the image | ![Grayscale](./Images/1.grayscale.png) | Converts the given image to grayscale. |
| 2. Smoothing | ![Smoothing](./Images/2.blur.png) | Smooths the image using Sobel operator to reduce noise. |
| 3.1. Edge detection | | Canny algorithm to detect edges. |
| 3.2. Thresholding | ![Thresholding](./Images/3.canny.png) | Otsu thresholding to make a binary image. |
| 4. Morphological: close | ![Close](./Images/4.close.png) | Dilation to combine all edges into a single circle + erosion to return the circles back to original sizes. |
| 5. Morphological: open | ![Open](./Images/5.open.png) | Erosion to remove the noise and dilation to return the circles back to original sizes. |
| 6. Edge detection | ![Edge detection](./Images/6.edges.png) | Canny edge detection again to leave edges only. |
| 7. Morphological: dilate | ![Dilate](./Images/7.dilate.png) | Dilation to thicker the edges. |
| 8. Hough Circles | ![Hough Circles](./Images/8.circles.png) | Hough circles transform to detect circles. |


## Model Results

And the most interesting part: the results part.

*Note: To reproduce the same behaviour run the following command:*

```text
./bin/CoinsDetector ../../Samples -c ../../Samples/circles.txt
```

Overall there are __154 coins__ in the given test set. The metrics are:

|Metric|Value|
|---|---|
|True positive|135|
|True negative|0 **the value is expected as we don't have such a group in our dataset**|
|False positive|9|
|False negative|19|
|Precission|0.9375|
|Recall|0.876623|
|F1 Score|0.90604|

*As was agreed in the lecture, the model detects a few circles incorrectly and keeps the score below 100% perfect.*

You can also see a similar report in your terminal when you run the application using your own equipment.

Raw report:

```text
01.jpg                                            
File path: ../../Samples/01.jpg                              
Detected: 8     circles.
#001. Center{x=340, y=474}, Radius{54}.
#002. Center{x=596, y=494}, Radius{102}.
#003. Center{x=186, y=180}, Radius{66}.
#004. Center{x=170, y=612}, Radius{81}.
#005. Center{x=392, y=292}, Radius{91}.
#006. Center{x=424, y=636}, Radius{63}.
#007. Center{x=170, y=366}, Radius{83}.
#008. Center{x=614, y=194}, Radius{100}.

02.jpg                                            
File path: ../../Samples/02.jpg                              
Detected: 9     circles.
#001. Center{x=140, y=312}, Radius{33}.
#002. Center{x=50, y=302}, Radius{41}.
#003. Center{x=312, y=188}, Radius{47}.
#004. Center{x=62, y=62}, Radius{58}.
#005. Center{x=310, y=62}, Radius{57}.
#006. Center{x=184, y=54}, Radius{44}.
#007. Center{x=228, y=308}, Radius{37}.
#008. Center{x=60, y=186}, Radius{47}.
#009. Center{x=180, y=182}, Radius{66}.

03.jpg                                            
File path: ../../Samples/03.jpg                              
Detected: 4     circles.
#001. Center{x=664, y=198}, Radius{49}.
#002. Center{x=174, y=190}, Radius{55}.
#003. Center{x=522, y=198}, Radius{65}.
#004. Center{x=338, y=196}, Radius{92}.

04.jpg                                            
File path: ../../Samples/04.jpg                              
Detected: 10    circles.
#001. Center{x=318, y=570}, Radius{85}.
#002. Center{x=102, y=98}, Radius{71}.
#003. Center{x=142, y=654}, Radius{106}.
#004. Center{x=656, y=330}, Radius{89}.
#005. Center{x=598, y=130}, Radius{107}.
#006. Center{x=450, y=404}, Radius{114}.
#007. Center{x=454, y=698}, Radius{87}.
#008. Center{x=162, y=376}, Radius{139}.
#009. Center{x=656, y=568}, Radius{119}.
#010. Center{x=326, y=152}, Radius{132}.

05.jpg                                            
File path: ../../Samples/05.jpg                              
Detected: 1     circles.
#001. Center{x=484, y=356}, Radius{181}.

06.jpeg                                           
File path: ../../Samples/06.jpeg                             
Detected: 5     circles.
#001. Center{x=406, y=104}, Radius{96}.
#002. Center{x=314, y=224}, Radius{47}.
#003. Center{x=226, y=330}, Radius{94}.
#004. Center{x=418, y=322}, Radius{86}.
#005. Center{x=218, y=102}, Radius{98}.

07.jpg                                            
File path: ../../Samples/07.jpg                              
Detected: 3     circles.
#001. Center{x=360, y=498}, Radius{89}.
#002. Center{x=638, y=508}, Radius{91}.
#003. Center{x=636, y=238}, Radius{127}.

08.jpg                                            
File path: ../../Samples/08.jpg                              
Detected: 3     circles.
#001. Center{x=144, y=270}, Radius{100}.
#002. Center{x=456, y=274}, Radius{90}.
#003. Center{x=332, y=146}, Radius{98}.

09.jpg                                            
File path: ../../Samples/09.jpg                              
Detected: 3     circles.
#001. Center{x=952, y=316}, Radius{48}.
#002. Center{x=210, y=278}, Radius{172}.
#003. Center{x=532, y=308}, Radius{190}.

10.jpg                                            
File path: ../../Samples/10.jpg                              
Detected: 2     circles.
#001. Center{x=402, y=204}, Radius{116}.
#002. Center{x=410, y=540}, Radius{189}.

11.jpg                                            
File path: ../../Samples/11.jpg                              
Detected: 5     circles.
#001. Center{x=140, y=170}, Radius{110}.
#002. Center{x=652, y=206}, Radius{110}.
#003. Center{x=604, y=488}, Radius{138}.
#004. Center{x=368, y=304}, Radius{111}.
#005. Center{x=230, y=586}, Radius{180}.

12.jpg                                            
File path: ../../Samples/12.jpg                              
Detected: 1     circles.
#001. Center{x=384, y=278}, Radius{213}.

13.jpg                                            
File path: ../../Samples/13.jpg                              
Detected: 1     circles.
#001. Center{x=118, y=128}, Radius{87}.

14.jpg                                            
File path: ../../Samples/14.jpg                              
Detected: 8     circles.
#001. Center{x=510, y=84}, Radius{50}.
#002. Center{x=378, y=90}, Radius{42}.
#003. Center{x=96, y=86}, Radius{45}.
#004. Center{x=378, y=228}, Radius{42}.
#005. Center{x=240, y=226}, Radius{50}.
#006. Center{x=96, y=226}, Radius{46}.
#007. Center{x=240, y=90}, Radius{50}.
#008. Center{x=512, y=228}, Radius{53}.

15.jpg                                            
File path: ../../Samples/15.jpg                              
Detected: 6     circles.
#001. Center{x=106, y=108}, Radius{51}.
#002. Center{x=300, y=114}, Radius{51}.
#003. Center{x=104, y=278}, Radius{50}.
#004. Center{x=522, y=114}, Radius{51}.
#005. Center{x=308, y=278}, Radius{50}.
#006. Center{x=512, y=270}, Radius{63}.

16.jpeg                                           
File path: ../../Samples/16.jpeg                             
Detected: 4     circles.
#001. Center{x=532, y=128}, Radius{70}.
#002. Center{x=618, y=358}, Radius{64}.
#003. Center{x=306, y=276}, Radius{60}.
#004. Center{x=770, y=142}, Radius{74}.

17.jpg                                            
File path: ../../Samples/17.jpg                              
Detected: 5     circles.
#001. Center{x=492, y=490}, Radius{38}.
#002. Center{x=270, y=598}, Radius{144}.
#003. Center{x=560, y=600}, Radius{141}.
#004. Center{x=244, y=236}, Radius{177}.
#005. Center{x=562, y=246}, Radius{182}.

18.jpg                                            
File path: ../../Samples/18.jpg                              
Detected: 7     circles.
#001. Center{x=458, y=954}, Radius{68}.
#002. Center{x=432, y=1254}, Radius{90}.
#003. Center{x=908, y=646}, Radius{94}.
#004. Center{x=908, y=330}, Radius{83}.
#005. Center{x=432, y=364}, Radius{76}.
#006. Center{x=440, y=632}, Radius{92}.
#007. Center{x=904, y=1216}, Radius{113}.

19.jpg                                            
File path: ../../Samples/19.jpg                              
Detected: 2     circles.
#001. Center{x=150, y=216}, Radius{125}.
#002. Center{x=446, y=208}, Radius{137}.

20.jpg                                            
File path: ../../Samples/20.jpg                              
Detected: 9     circles.
#001. Center{x=514, y=516}, Radius{158}.
#002. Center{x=170, y=506}, Radius{145}.
#003. Center{x=840, y=506}, Radius{145}.
#004. Center{x=182, y=846}, Radius{159}.
#005. Center{x=506, y=848}, Radius{149}.
#006. Center{x=506, y=176}, Radius{156}.
#007. Center{x=178, y=188}, Radius{156}.
#008. Center{x=840, y=838}, Radius{165}.
#009. Center{x=852, y=180}, Radius{153}.

21.jpg                                            
File path: ../../Samples/21.jpg                              
Detected: 1     circles.
#001. Center{x=478, y=244}, Radius{235}.

22.jpg                                            
File path: ../../Samples/22.jpg                              
Detected: 2     circles.
#001. Center{x=412, y=156}, Radius{62}.
#002. Center{x=202, y=136}, Radius{104}.

23.jpg                                            
File path: ../../Samples/23.jpg                              
Detected: 15    circles.
#001. Center{x=640, y=258}, Radius{77}.
#002. Center{x=334, y=96}, Radius{58}.
#003. Center{x=490, y=96}, Radius{67}.
#004. Center{x=330, y=264}, Radius{64}.
#005. Center{x=614, y=90}, Radius{67}.
#006. Center{x=504, y=258}, Radius{75}.
#007. Center{x=308, y=450}, Radius{79}.
#008. Center{x=230, y=100}, Radius{48}.
#009. Center{x=304, y=654}, Radius{103}.
#010. Center{x=198, y=264}, Radius{67}.
#011. Center{x=640, y=452}, Radius{86}.
#012. Center{x=178, y=450}, Radius{84}.
#013. Center{x=158, y=658}, Radius{110}.
#014. Center{x=508, y=444}, Radius{83}.
#015. Center{x=516, y=658}, Radius{90}.

24.png                                            
File path: ../../Samples/24.png                              
Detected: 19    circles.
#001. Center{x=250, y=42}, Radius{45}.
#002. Center{x=150, y=296}, Radius{38}.
#003. Center{x=448, y=298}, Radius{45}.
#004. Center{x=346, y=124}, Radius{38}.
#005. Center{x=48, y=216}, Radius{38}.
#006. Center{x=444, y=216}, Radius{46}.
#007. Center{x=444, y=124}, Radius{38}.
#008. Center{x=150, y=218}, Radius{45}.
#009. Center{x=50, y=44}, Radius{45}.
#010. Center{x=350, y=218}, Radius{45}.
#011. Center{x=444, y=42}, Radius{38}.
#012. Center{x=252, y=218}, Radius{45}.
#013. Center{x=48, y=128}, Radius{38}.
#014. Center{x=148, y=122}, Radius{38}.
#015. Center{x=252, y=122}, Radius{39}.
#016. Center{x=50, y=296}, Radius{45}.
#017. Center{x=148, y=42}, Radius{45}.
#018. Center{x=348, y=42}, Radius{38}.
#019. Center{x=250, y=296}, Radius{45}.

25.jpg                                            
File path: ../../Samples/25.jpg                              
Detected: 5     circles.
#001. Center{x=268, y=248}, Radius{57}.
#002. Center{x=454, y=130}, Radius{68}.
#003. Center{x=390, y=252}, Radius{57}.
#004. Center{x=290, y=88}, Radius{83}.
#005. Center{x=116, y=154}, Radius{96}.

26.webp                                           
File path: ../../Samples/26.webp                             
Detected: 6     circles.
#001. Center{x=514, y=290}, Radius{34}.
#002. Center{x=460, y=194}, Radius{38}.
#003. Center{x=460, y=100}, Radius{34}.
#004. Center{x=364, y=194}, Radius{34}.
#005. Center{x=264, y=196}, Radius{41}.
#006. Center{x=264, y=98}, Radius{38}.

P.S.: Calculating metrics may take a while.

The test set has 154 coins.

Confusion matrix values:
true positive       135                 
true negative       0                   
false positive      9                   
false negative      19                  

Prediction metrics:
precision           0.9375              
recall              0.876623            
f1                  0.90604
```

### Detected circles example

This section just shows a few final result.

| Sample 9 | Sample 14 |
| --- | --- |
| ![Detected Coins 9](./Images/detection.sample.1.jpg)| ![Detected Coins 14](Images/detection.sample.2.jpg) |

### Images where the model fails

My model fails to detect coins that are located on noisy background (non-monotonic).

| Sample 2 | Sample 4 |
| --- | --- |
| ![Detected Coins 2](./Images/invalid_02.jpg)| ![Detected Coins 4](./Images/invalid_04.webp) |


Wow, if you read this far you're a real hero. 
*Hopefully, you've enjoyed the reading the document. Thank you and good luck!*

## License

```text
MIT License

Copyright (c) 2022 Alex Dadukin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

```
