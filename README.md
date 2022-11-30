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
./bin/CoinDetector image1 image_folder1 image2 ... imageN [-o output prefix] [-c coins config] [-d debug] [-dc draw config coins]
```

However, it can be a bit confusing. Let me breifly explain what these arguments do. Basically, the invocation consists of two important groups: an images list and additional arguments.

1. **Images list**: just a list of either image files or folders with images. If an entry is an images folder then all images are recursively extracted from within it.

2. **Additional arguments**: all arguments are optional. If you decide to use them read thoroughly the infromation in the table below.

|Argument|Desciption|
|-----|-----|
|-d| *Debug flag*: if specified then the application displays additional processing steps, otherwise only result image with detected coins will be shown. |
|-o| *Output files prefix*: specifies the final directory for output images. If not specified then the final images will be saved in the application folder, ie the folder from where the application is located. |
|-c| *Coins config file*: if specified then the passed images will be tested against this config and prediction metrics will be calculated. Config should contain information about coins in the given images. To create a custom config see [Config section](Config). |
|-dc| *Draw config coins*: if specified coins from config will be displayed as **blue** circles on the final image. |

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
| 2. Smoothing | ![Smoothing](./Images/2.smoothing.png) | Smooths the image using Sobel operator to reduce noise. |
| 3. Edge detection | ![Canny](./Images/3.canny.png) | Canny algorithm to detect edges. |
| 4. Thresholding | ![Thresholding](./Images/4.threshold.png) | Otsu thresholding to make a binary image. |
| 5.1 Morphological: dilate |  | Agressive dilation to combine all edges into a single circle. |
| 5.2 Morphological: open | ![Close](./Images/5.morphology.png) | Erosion to remove the noise and dilation to return the circles back to original sizes. |
| 6. Contour | ![Contour](./Images/6.contour.png) | Contours detection to separate contours of circles and draw them with a given thickness. |
| 7. Smoothing | ![Smoothing](./Images/7.smoothing.png) | Smoothing to reduce noise. |
| 8.1 Morphological: close | ![Smoothing](./Images/8.close.png) | Dilation followed by errosion to merge for the last time all separated objects. |
| 8.2 Morphological: open | ![Smoothing](./Images/9.open.png) | Errosion followed by dilation to reduce noise if left. |
| 8.3 Morphological: dilate | ![Smoothing](./Images/10.dilate.png) | Dilation to make contours thicker. |
| 9. Hough Circles | ![Hough Circles](./Images/11.result.png) | Hough circles transform to detect circles. |


## Model Results

And the most interesting part: the results part.

*Note: To reproduce the same behaviour run the following command:*

```text
./bin/CoinsDetector ../../Samples -c ../../Samples/circles.txt
```

Overall there are __173 coins__ in the given test set. The metrics are:

|Metric|Value|
|---|---|
|True positive|161|
|True negative|0 **the value is expected as we don't have such a group in our dataset**|
|False positive|12|
|False negative|12|
|Precission|0.930636|
|Recall|0.930636|
|F1 Score|0.930636|

*As was agreed in the lecture, the model detects a few circles incorrectly and keeps the score below 100% perfect.*

You can also see a similar report in your terminal when you run the application using your own equipment.

Final report with the images is located in [Report/Samples folder](./Report/Samples/).

### Detected circles example

This section just shows a few final result.

| Sample 4 | Sample 9 | Sample 14 |
| --- | --- | --- |
| ![Detected Coins 4](./Images/detection.sample.3.jpg) | ![Detected Coins 9](./Images/detection.sample.1.jpg)| ![Detected Coins 14](Images/detection.sample.2.jpg) |

Also I attached the results of the coins from lectures

| Sample 1 | Sample 2 |
| --- | --- |
| ![Lecture Sample 1](./Images/lecture.1.png) | ![Lecture Sample 2](./Images/lecture.2.png)|

**Additional test images can be found in [Additional](./Additional/).**

### Images where the model fails

My model fails to detect coins that are located on a noisy background (non-monochromatic) or with coins captured using extreme angles.

Such examples are given below:

| Sample 2 | Sample 4 |
| --- | --- |
| ![Detected Coins 2](./Images/invalid_01.jpg)| ![Detected Coins 4](./Images/invalid_04.jpg) |


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
