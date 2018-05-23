# Smart Separate App

Smart Separate is an app that processes images by carving out selected object and put it on various backgrounds. Its algorithm uses [OpenCV](https://opencv.org/) library contour feature. User can input files by loading a existing image or take a photo. The user can also save the processed image to desired path.

## Getting Started

This app is built on [openFrameworks](https://github.com/openframeworks/openFrameworks), so if you don't have that downloaded, please check it out and set it up.

### Prerequisites

A list of openFrameworks add-ons that are needed:
- [ofxCv](https://github.com/kylemcdonald/ofxCv/tree/stable)
- [ofxFileDialog](https://github.com/geoffdonaldson/ofxFileDialog)
- [ofxParagraph](https://github.com/braitsch/ofxParagraph)

### Installing

After installing the prerequisites above, we can install this app with the following steps:

1. Clone this repository or download zip into the `openFrameworks/apps/` folder, where openFrameworks is the folder of all the openFrameworks file.

2. Open the application in the following path `openFrameworks/projectGenerator/projectGenerator`.
  - Besides Project Name, click on `import`, and select the folder of this app you just downloaded.
  - Click on `Generate`.
  - Click on `Open in IDE`.

3. Run this app with your IDE.

## Usage

When you first open the app, a menu looks like the following will show up.

![menu dialog](https://raw.githubusercontent.com/Xiangmingchen/SmartSeparateImages/master/Menu.png)

If you click `Load Image` and select a local image, you will see the following window.

![image process dialog](https://raw.githubusercontent.com/Xiangmingchen/SmartSeparateImages/master/ImageProcessDialog.png)

Where the image on the left will be replaced by your choice of image. The image will be processed once you click on it.

The buttons on the right has the following functionalities:

- **Reset:** Reset the image back to original.
- **Background arrows:** Click on the left and right arrow after processing the image. The white default background will be replaced by patterned images.
- **Save Image:** Save the processed image to a specific path.
- **Load Image:** Load another image. Any new change of the current image will be discarded.
- **Use Camera:** Turn on the camera and take a picture to process.

Possible processed result can look like the following.

![image process result](https://raw.githubusercontent.com/Xiangmingchen/SmartSeparateImages/master/ImageProceesResult.png)

When the processed image doesn't look satisfactory, try click on different color points.

After having fun with the image, you can save it using the `Save Image` button.

If you want to take a picture, you can click on the `Use Camera` button, and it will redirect you to a page looks like the following.

![Camera open](https://raw.githubusercontent.com/Xiangmingchen/SmartSeparateImages/master/CameraNotTaken.png)

A picture will be taken when you click on `OK`. Then you can use this picture or retake one.

![Camera taken picture](https://raw.githubusercontent.com/Xiangmingchen/SmartSeparateImages/master/CameraTaken.png)

## Further Improvement

This app can be further improved by adding the following features:

- Real time image processing when camera is turned on (This is currently impossible because the runtime of the algorithm is really slow).
- Change threshold of contour when loading different images, so that objects with similar color as background can also be precisely contoured
- User can upload personal backgrounds.

## Acknowledgments

[Background picture sources](http://animalia-life.club/other/background-tumblr-pattern.html)
