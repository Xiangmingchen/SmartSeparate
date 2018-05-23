#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxGui.h"
#include "ofxFileDialog.h"
#include "ofAppGLFWWindow.h"
#include "ofxParagraph.h"
#include "roundedButton.h"

// A application that process image by carving out objects that are selected by user
class ofSmartSeparate : public ofBaseApp {
public:
    // states of the app
    enum State {
        MENU = 0,
        CAMERA,
        IMAGE
    };
    
    // constructor that takes in the screen size in order to adjust
    // the initial window size and position.
    ofSmartSeparate(ofPoint in_screen_size = {1280, 720});
    
	void setup();
	void update();
	void draw();
	void mousePressed(int x, int y, int button);
    void windowResized(int w, int h);
    
//private:  // the folloing functions and variables should all be private if published
//          //  But for testing purposes they are made public
    // draw function for different states
    void DrawCamera_();
    void DrawMenu_();
    void DrawImage_();
    
    // functions called when buttons are pressed
    void CameraButtonSelected_();
    void LoadImageSelected_();
    void SwitchToImageMode_();
    void ImageClicked_(int x, int y);
    
    // image processing
    void ExtendHighlight_(queue<ofPoint>& targets);         // Extend highlight area
    void FindMoreTargets_(vector<ofPoint>& new_highlight);  // find more targets based on contour_
    bool FindNewHighlights_(vector<ofPoint>& new_highlight,
                            ofPoint curr_position);         // find newly highlighted pixels
    void ProcessImage_(int x, int y);       // process image based on point
    void RenderHighlight_();                // put highlighted area onto processed img
    void UpdateBackground_();               // update processed img and bg_number based on curr_bg_n
    void SetBackground_();                  // set processed img to background
    void ResizeImage_();                    // resize original image to be within kMaxImgSize
    void SetUpContourFinder_(int threshold);// contuor finder setup basedo on threshold
    void LoadBackgroundImgs_();             // load background images
    int FlattenedIndex_(int x, int y);      // flatten a 2D index into 1D
    vector<bool> is_highlighted_;           // record highlighted area
    vector<ofPoint> highlight_area_;        // coordinates of highlighted area
    std::queue<ofPoint> targets_;           // target points to be contoured
    
    State current_state_;
    dialogSettings setting;
    int curr_bg_number_;
    
    // values used to find contour
    bool clicked_;
    bool processing_;
    ofPoint target_;
    ofPolyline contour_;
	ofVideoGrabber camera_;
    string image_name_;
    ofColor target_color_;
    ofImage original_image_;
    ofImage processed_image_;
    ofRectangle image_rect_;
	ofxCv::ContourFinder contour_finder_;
    vector<ofImage> backgrounds_;
    
    // flag of whether a picture has been taken or not in CAMERA mode
    bool picture_taken_;
    
    // buttons: first word denotes the state that it appears,
    //          second word denotes the change to make
    RoundedButton menu_tocamera_button_;
    RoundedButton menu_toimage_button_;
    RoundedButton image_reset_button_;
    RoundedButton image_left_arro_button_;
    RoundedButton image_right_arro_button_;
    RoundedButton image_saveimage_button_;
    RoundedButton image_loadanother_button_;
    RoundedButton image_tocamera_button_;
    RoundedButton camera_takephoto_button_;
    RoundedButton camera_retakephoto_button_;
    RoundedButton camera_toimage_button_;
    vector<RoundedButton*> image_buttons_;
    
    // texts that are implemented with buttons
    RoundedButton image_click_image_txt_;
    RoundedButton image_background_txt_;
    RoundedButton image_bg_number_txt_;
    RoundedButton image_processing_overlap_;
    
    // position variables
    ofPoint screen_size_;
    double image_offset_x;
    double image_offset_y;
    double image_width_;
    double image_height_;
    double image_ratio_;
    double image_outline_ratio_;
    
    // updates position variables based on new window width and height
    void UpdatePositionOffsets_(int window_w, int window_h);
    double YOffset_(double numerator);
    void SetUpButtons_();
    
    // constant colors
    ofColor kLightPink = ofColor(249, 226, 214);
    ofColor kDarkPink = ofColor(226, 182, 170);
    ofColor kDarkGreen = ofColor(61, 74, 57);
    ofColor kLightGreen = ofColor(143, 159, 136);
    ofColor kOffWhite = ofColor(239, 238, 237);
    ofColor kWhite = ofColor(255, 255, 255);
    ofColor kTransparent = ofColor(0, 0, 0, 0);
    ofColor kGray = ofColor(170, 170, 170);
    
    // constant strings
    const string kTitleFont = "Neon.ttf";
    const string kContentFont = "CaviarDreams.ttf";
    const string kArrowFont = "Patchy Robots.ttf";
    const string kDefaultBgString = "N\\A";
    const vector<string> kAllowedFileType = {"png", "jpg", "jpeg", "gif"};
    
    // Constants used for processing image
    const int kMaxImgSize = 600;
    const int kContourThreshold = 51;
    const int kExtendThreshold = 90;
    const int kContourWidth = 2;
    const int kMinAreaR = 10;
    const int kSmoothSize = 7;
    const int kTotalBgNumber = 17;
    const int kDefaultBgNum = -1;
    double kEdgeInterval = 0.21;
    
    // Scalers for window, image, and string size
    double kWindowSizeScaler = 2.0 / 3.0;
    double kWindowPositionOffset = 1.0 / 6.0;
    double kImageSizeScalerW = 13.0 / 24.0;
    double kImageSizeScalerH = 7.0 / 8.0;
    double kImageOffsetScalerX = 1.0 / 16.0;
    double kImageOffsetScalerY = 1.0 / 16.0;
    double kStringTitleScaler = 0.03;
    double kStringContentScaler = 0.02;
    double kStringProcessingScaler = 0.05;
    
    // round factor for different size button
    double kBtnRoundBig = 1.0 / 32.0;
    double kBtnRoundSmall = 1.0 / 64.0;
    
    // MENU mode
    // menu button dimensions
    double kMenuBtnWidth = 2.0 / 7.0;
    double kMenuBtnHeight = 1.0 / 2.0;
    double kMenuBtnTop = 1.0 / 4.0;
    double kMenuBtnLeft1 = 1.0 / 7.0;
    double kMenuBtnLeft2 = 4.0 / 7.0;
    
    // IMAGE mode
    // Reset button
    double kBtnResetWidth = 1.0 / 6.0;
    double kBtnResetHeight = 3.0 / 32.0;
    double kBtnResetXOffset = 23.0 / 32.0;
    // Arrows
    double kBtnArrowWidth = 1.0 / 10.0;
    double kBtnArrowHeight = 6.0 / 32.0;
    double kBtnLeftArroXOffset = 2.0 / 3.0 + 1.0 / 60.0 - 1.0 / 32.0;
    double kBtnRightArroXOffset = 2.0 / 3.0 + 1.0 / 60.0 + 3.0 / 15.0 - 1.0 / 32.0;
    // Bottom three buttons
    double kBtnBottomGroupWidth = 1.0 / 4.0;
    double kBtnBottomGroupHeight = 3.0 / 35.0;
    double kBtnBottomGroupXOffset = 65.0 / 96.0;
    // Titles / captions
    double kTextTitleWidth = 1.0 / 2.0;
    double kTextTitleHeight = 4.0 / 32.0;
    double kTextTitleXOffset = 7.0 / 12.0 - 1.0 / 32.0;
    double kClickImageYOffset = 1.0 / 32.0;
    double kBackgroundYOffset = 10.0 / 32.0;
    // Background number display
    double kBgNumberXOffset = 4.0 / 5.0 - 1.0 / 32.0;
    double kBgNumberYOffset = 15.0 / 32.0;
    double kBgNumberWidth = 1.0 / 15.0;
    double kBgNumberHeight = 4.0 / 32.0;
    
    // CAMERA mode
    // "OK" button that takes picture
    double kBtnTakePicWidth = 3.0 / 32.0;
    double kBtnTakePicHeight = 1.0 / 7.0;
    double kBtnTakePicXOffSet = 28.0 / 32.0;
    double kBtnTakePicYOffSet = 7.0 / 16.0;
    // Retake photo button
    double kBtnRetakePicWidth = 1.0 / 6.0;
    double kBtnRetakePicXOffset = 29.0 / 36.0;
    double kBtnRetakePicYOffset = 13.0 / 16.0;
    double kBtnUseThisPicXOffset = 1.0 / 36.0;
    
    // The following YOffsets should be divided by YDivisor to get real Y offset
    double kSidebarYDivisor = 32.0;
    double kTextClickYOffset = 1.0;
    double kBtnResetYOffset = 6.0;
    double kTextBgYOffset = 11.0;
    double kBtnArroYOffset = 14.0;
    double kTextBgNumYOffset = 15.0;
    double kBtnSaveImgYOffset = 22.0;
    double kBtnLoadImgYOffset = 25.0;
    double kBtnUseCamYOffset = 28.0;
};
