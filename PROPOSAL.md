# Smart-Separate Project Proposal
When people are taking pictures, they usually want to focus on specific subjects in the picture. However, the background distracts people from the main subject at times.
Therefore I want to create a application that can filter out the background and only leave the main subject on the picture.

## Usage
The user can open a local picture, or turn on the camera and process the video input int real time. The main subject should be selected by being tapped on, and then everything else in the picture will be filtered out. User can choose which background pattern to be applied on the empty area by pressing arrow keys. Then to undo the select, user can tap on the background area, so that he / she can select another one.

## Library
To achieve object contouring, I will use the c++ library [OpenCV](https://opencv.org/). Other libraries may be used for image processing and UI.

## Possible Milestones
1. Open local/static image and contour object.
2. Real time image processing.
3. Saving processed image and background pattern choices.
4. Extra features described below

## Further Extending
After selecting the object, since the background picture is probably very bright, the object can appear somewhat dim. Therefore to extend this project, a "image filter" can be added to brighten the object or process in other ways.

Another feature will be user can enter text or draw on the processed picture so that they can send to their friends.
