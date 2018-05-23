#include "ofSmartSeparate.h"

ofSmartSeparate::ofSmartSeparate(ofPoint in_screen_size) : ofBaseApp() {
    screen_size_ = in_screen_size;
}

void ofSmartSeparate::setup() {
    // put the window in the middle of the screen
    ofSetWindowShape(screen_size_.x * kWindowSizeScaler,
                     screen_size_.y * kWindowSizeScaler);
    ofSetWindowPosition(screen_size_.x * kWindowPositionOffset,
                        screen_size_.y * kWindowPositionOffset);
    
    // set up the buttons, bg images, and bg color
    SetUpButtons_();
    LoadBackgroundImgs_();
    ofBackground(kLightPink);
    
    // set up file dialog setting, so that only pictures are allowed
    setting.allowMultipleSelection = false;
    for (string file_type : kAllowedFileType) {
        setting.fileTypes.push_back(file_type);
    }
    
    current_state_ = MENU;
    curr_bg_number_ = kDefaultBgNum;
    image_outline_ratio_ = ofGetWidth() * kImageSizeScalerW / (ofGetHeight() * kImageSizeScalerH);
}

void ofSmartSeparate::LoadBackgroundImgs_() {
    backgrounds_.resize(kTotalBgNumber);
    for (int i = 0; i < kTotalBgNumber; i++) {
        backgrounds_[i].load("bg" + to_string(i) + ".jpg");
    }
}

double ofSmartSeparate::YOffset_(double numerator) {
    return numerator / kSidebarYDivisor;
}

// Resize the image to make sure it's under kMaxImgSize * kMaxImgSize
void ofSmartSeparate::ResizeImage_() {
    // if width is greater than height, maximize by width
    if (image_ratio_ > 1 && original_image_.getWidth() > kMaxImgSize) {
        double new_height = kMaxImgSize / image_ratio_;
        original_image_.resize(kMaxImgSize, new_height);
    }
    // if the height is greater than width, mazimize by height
    else if (image_ratio_ <= 1 && original_image_.getHeight() > kMaxImgSize) {
        double new_width = kMaxImgSize * image_ratio_;
        original_image_.resize(new_width, kMaxImgSize);
    }
}

void ofSmartSeparate::update() {
    switch (current_state_) {
        case CAMERA:
            if (!picture_taken_) {
                camera_.update();
            }
            break;
            
        case IMAGE:
            // keep process the image until there is no more target points
            if (processing_) {
                ExtendHighlight_(targets_);
                RenderHighlight_();
                processing_ = !targets_.empty();
            }
            break;
            
        default:
            break;
    }
}

void ofSmartSeparate::draw() {
    switch (current_state_) {
        case MENU:
            DrawMenu_();
            break;
            
        case CAMERA:
            DrawCamera_();
            break;
            
        case IMAGE:
            DrawImage_();
            break;
    }
}

void ofSmartSeparate::DrawMenu_() {
    // draw the two buttons
    menu_tocamera_button_.draw();
    menu_toimage_button_.draw();
}

void ofSmartSeparate::DrawCamera_() {
    // draw the camera with buttons
    ofSetColor(kWhite);
    camera_.draw(0, 0);
    if (picture_taken_) {
        camera_retakephoto_button_.draw();
        camera_toimage_button_.draw();
    } else {
        camera_takephoto_button_.draw();
    }
}

void ofSmartSeparate::DrawImage_() {
    // draw the processed image
    ofSetColor(kWhite);
    processed_image_.update();
    processed_image_.draw(image_offset_x, image_offset_y, image_width_, image_height_);
    
    // draw the buttons and texts
    for (auto button : image_buttons_) {
        button->draw();
    }
    
    // if processing, draw the overlap gray layer
    if (processing_) {
        image_processing_overlap_.draw();
    }
}

void ofSmartSeparate::CameraButtonSelected_() {
    // turn on the camera and resize the window
    current_state_ = CAMERA;
    picture_taken_ = false;
    camera_.setup(screen_size_.x, screen_size_.y);
    ofSetWindowShape(camera_.getWidth(), camera_.getHeight());
    ofSetWindowPosition(0, 0);
}

void ofSmartSeparate::LoadImageSelected_() {
    // open a file dialog
    std::vector<std::string> files = openDialog(setting);
    
    // load the image if the user chose a image
    if (files.size() > 0) {
        // load the image
        original_image_.load(files[0]);
        image_name_ = files[0].substr(files[0].rfind("/") + 1);
        
        SwitchToImageMode_();
    }
}

// do preparation for process image
void ofSmartSeparate::SwitchToImageMode_() {
    // set up current states
    current_state_ = IMAGE;
    clicked_ = false;
    processing_ = false;
    
    // set up for image process
    image_ratio_ = original_image_.getWidth() / (double) original_image_.getHeight();
    ResizeImage_();
    is_highlighted_.resize(original_image_.getHeight() * original_image_.getWidth());
    processed_image_ = original_image_;
    SetUpContourFinder_(kContourThreshold);
    windowResized(ofGetWidth(), ofGetHeight());
}

// use the current image width as width, calculate the idx by: x + y * width
int ofSmartSeparate::FlattenedIndex_(int x, int y) {
    return x + y * original_image_.getWidth();
}

void ofSmartSeparate::ProcessImage_(int x, int y) {
    clicked_ = true;
    processing_ = true;
    
    // set new target
    target_.set(x, y);
    target_color_ = original_image_.getPixels().getColor(x, y);
    targets_.push(target_);
    
    // clear previous highlight
    highlight_area_.clear();
    is_highlighted_.assign(is_highlighted_.size(), false);
    SetBackground_();
}

void ofSmartSeparate::ExtendHighlight_(std::queue<ofPoint>& targets) {
    // set up new target point
    ofPoint curr_position = targets_.front();
    ofColor curr_color = original_image_.getColor(curr_position.x, curr_position.y);
    targets.pop();
    contour_finder_.setTargetColor(curr_color);
    contour_finder_.findContours(original_image_);
    
    vector<ofPoint> new_highlight;
    bool found_new_point = FindNewHighlights_(new_highlight, curr_position);
    
    // if current contour is within highlight, the current extension is done
    if (!found_new_point) {
        return;
    }
    
    // otherwise find more target points along the new outline
    FindMoreTargets_(new_highlight);
    
    // highlight the new area
    for (unsigned i = 0; i < new_highlight.size(); i++) {
        is_highlighted_[FlattenedIndex_(new_highlight[i].x, new_highlight[i].y)] = true;
        highlight_area_.push_back(new_highlight[i]);
    }
}

bool ofSmartSeparate::FindNewHighlights_(vector<ofPoint>& new_highlight, ofPoint curr_position) {
    // find the contour that contains the target
    cv::Rect bonding_rect;
    for (unsigned int i = 0; i < contour_finder_.size(); i++) {
        auto& curr_contour = contour_finder_.getPolyline(i);
        if (curr_contour.inside(curr_position)) {
            contour_ = curr_contour.getSmoothed(kSmoothSize);
            bonding_rect = contour_finder_.getBoundingRect(i);
            break;
        }
    }
    
    bool found_new_point = false;
    // put the pixels that are in new contour into new_highlight
    for (unsigned int x = bonding_rect.x; x < bonding_rect.x + bonding_rect.width; x++) {
        for (unsigned int y = bonding_rect.y; y < bonding_rect.y + bonding_rect.height; y++) {
            if (contour_.inside(x, y) && !is_highlighted_[FlattenedIndex_(x, y)]) {
                new_highlight.push_back(ofPoint(x, y));
                found_new_point = true;
            }
        }
    }
    
    return found_new_point;
}

void ofSmartSeparate::FindMoreTargets_(vector<ofPoint>& new_highlight) {
    // pick a point along the new contour edge and push it into the queue
    ofPoint target_cp(target_color_.getHue(), target_color_.getSaturation(),
                      target_color_.getBrightness());
    int count = 0;
    for (double i = 0; i < 1; i += kEdgeInterval) {
        ofPoint edge_pt = contour_.getPointAtLength(i * contour_.getPerimeter());
        if (is_highlighted_[FlattenedIndex_(edge_pt.x, edge_pt.y)]) {
            continue;
        }
        
        // only push the point if it is significantly different from current target
        ofColor edge_color = original_image_.getColor(edge_pt.x, edge_pt.y);
        ofPoint edge_cp(edge_color.getHue(), edge_color.getSaturation(),
                        edge_color.getBrightness());
        double distance = target_cp.distance(edge_cp);
        if (distance < kExtendThreshold) {
            targets_.push(edge_pt);
            new_highlight.push_back(edge_pt);
            count++;
        }
        
        // for each contour, we only find two points. If we look for more, the runtime
        // will be really slow because it's exponential. If we look for fewer, the highlight
        // will be smaller
        if (count > 1) {
            break;
        }
    }
}

void ofSmartSeparate::RenderHighlight_() {
    for (unsigned int i = 0; i < highlight_area_.size(); i++) {
        ofPoint& point = highlight_area_[i];
        processed_image_.setColor(point.x, point.y,
                                  original_image_.getColor(point.x, point.y));
    }
}

void ofSmartSeparate::mousePressed(int x, int y, int button) {
    switch (current_state_) {
        case MENU:
            // check if the buttons are pressed
            if (menu_tocamera_button_.contains(x, y)) {
                CameraButtonSelected_();
            } else if (menu_toimage_button_.contains(x, y)) {
                LoadImageSelected_();
            }
            break;
            
        case CAMERA:
            if (!picture_taken_ && camera_takephoto_button_.contains(x, y)) {
                picture_taken_ = true;
            } else if (picture_taken_ && camera_retakephoto_button_.contains(x, y)) {
                picture_taken_ = false;
            } else if (picture_taken_ && camera_toimage_button_.contains(x, y)) {
                original_image_.setFromPixels(camera_.getPixels());
                image_name_ = ofGetTimestampString() + ".jpg";
                SwitchToImageMode_();
            }
            break;
            
        case IMAGE:
            ImageClicked_(x, y);
            break;
    }
}

void ofSmartSeparate::ImageClicked_(int x, int y) {
    // if the image is clicked, process based on the target point
    if (image_rect_.inside(x, y) && !processing_) {
        double trans_x = (x - image_offset_x) * original_image_.getWidth() / image_width_;
        double trans_y = (y - image_offset_y) * original_image_.getHeight() / image_height_;
        ProcessImage_(trans_x, trans_y);
    }
    // reset button clicked
    else if (image_reset_button_.contains(x, y)) {
        processed_image_ = original_image_;
        clicked_ = false;
        processing_ = false;
    }
    // save current image, open up a save dialog
    else if (image_saveimage_button_.contains(x, y)) {
        ofFileDialogResult save_file_result = ofSystemSaveDialog(image_name_, "");
        if (save_file_result.bSuccess){
            
            // if the image is jpg or jpeg, need to set ImageType first,
            // otherwise it will save 0 byte image
            string file_type = image_name_.substr(image_name_.rfind(".") + 1);
            if (file_type == "jpg" || file_type == "jpeg") {
                processed_image_.setImageType(OF_IMAGE_COLOR);
            }
            processed_image_.save(save_file_result.filePath);
        }
    }
    // load image
    else if (image_loadanother_button_.contains(x, y)) {
        LoadImageSelected_();
    }
    // use camera
    else if (image_tocamera_button_.contains(x, y)) {
        CameraButtonSelected_();
    }
    // left arrow is clicked, changed the background
    else if (image_left_arro_button_.contains(x, y)) {
        curr_bg_number_--;
        if (curr_bg_number_ < kDefaultBgNum) {
            curr_bg_number_ = kTotalBgNumber - 1;
        }
        UpdateBackground_();
    }
    // right arrow
    else if (image_right_arro_button_.contains(x, y)) {
        curr_bg_number_++;
        if (curr_bg_number_ == kTotalBgNumber) {
            curr_bg_number_ = kDefaultBgNum;
        }
        UpdateBackground_();
    }
}

// set the entire processed image to the background image
void ofSmartSeparate::SetBackground_() {
    if (curr_bg_number_ == kDefaultBgNum) {
        processed_image_.setColor(kWhite);
    } else {
        for (int x = 0; x < processed_image_.getWidth(); x++) {
            for (int y = 0; y < processed_image_.getHeight(); y++) {
                processed_image_.setColor(x, y, backgrounds_[curr_bg_number_].getColor(x, y));
            }
        }
    }
}

void ofSmartSeparate::UpdateBackground_() {
    if (clicked_) {
        SetBackground_();
        RenderHighlight_();
    }
    
    // set the bg number on UI
    if (curr_bg_number_ == kDefaultBgNum) {
        image_bg_number_txt_.SetText(kDefaultBgString);
    } else {
        image_bg_number_txt_.SetText(to_string(curr_bg_number_ + 1));
    }
}

void ofSmartSeparate::windowResized(int w, int h) {
    switch (current_state_) {
        case MENU:
            menu_tocamera_button_.ResizeWindow(w, h);
            menu_toimage_button_.ResizeWindow(w, h);
            break;
            
        case CAMERA:
            camera_.setup(w, h);
            ofSetWindowShape(camera_.getWidth(), camera_.getHeight());
            camera_takephoto_button_.ResizeWindow(ofGetWidth(), ofGetHeight());
            camera_retakephoto_button_.ResizeWindow(ofGetWidth(), ofGetHeight());
            camera_toimage_button_.ResizeWindow(ofGetWidth(), ofGetHeight());
            break;
        
        case IMAGE:
            UpdatePositionOffsets_(w, h);
            for (auto button : image_buttons_) {
                button->ResizeWindow(w, h);
            }
            image_processing_overlap_.ResizeWindow(w, h);
            break;
    }
}

void ofSmartSeparate::SetUpContourFinder_(int threshold) {
    contour_finder_.setThreshold(threshold);
    contour_finder_.setFindHoles(false);
    contour_finder_.setMinArea(kMinAreaR);
    contour_finder_.setMaxArea(ofGetWidth() * ofGetHeight());
    contour_finder_.setTargetColor(target_color_, ofxCv::TRACK_COLOR_HS);
}

void ofSmartSeparate::UpdatePositionOffsets_(int window_w, int window_h) {
    image_offset_x = kImageOffsetScalerX * window_w;
    image_offset_y = kImageOffsetScalerY * window_h;
    image_outline_ratio_ = window_w * kImageSizeScalerW / (window_h * kImageSizeScalerH);
    
    // when the image is horizontally longer than its outline, maxmize it by width
    if (image_ratio_ > image_outline_ratio_) {
        image_width_ = window_w * kImageSizeScalerW;
        image_height_ = image_width_ / image_ratio_;
        image_offset_y = (window_h - image_height_) / 2.0;
    }
    // when the image is vertically longer, maximize it by height
    else {
        image_height_ = window_h * kImageSizeScalerH;
        image_width_ = image_height_ * image_ratio_;
        image_offset_x += (window_w * kImageSizeScalerW - image_width_) / 2.0;
    }
    image_rect_.set(image_offset_x, image_offset_y, image_width_, image_height_);
}

void ofSmartSeparate::SetUpButtons_() {
    double window_w = ofGetWidth();
    double window_h = ofGetHeight();
    
    // each button set up different width, height, x, y, and text
    // buttons in MENU mode
    menu_toimage_button_.setUp(window_w, window_h, kMenuBtnWidth, kMenuBtnHeight, kBtnRoundBig,
                               ofPoint(kMenuBtnLeft2, kMenuBtnTop), "Load Image", kLightGreen,
                               kTitleFont, kDarkGreen, kStringTitleScaler);
    menu_tocamera_button_.setUp(window_w, window_h, kMenuBtnWidth, kMenuBtnHeight, kBtnRoundBig,
                                ofPoint(kMenuBtnLeft1, kMenuBtnTop), "Use Camera", kLightGreen,
                                kTitleFont, kDarkGreen, kStringTitleScaler);
    // buttons in CAMERA mode
    camera_takephoto_button_.setUp(window_w, window_h, kBtnTakePicWidth, kBtnTakePicHeight,
                            kBtnRoundSmall, ofPoint(kBtnTakePicXOffSet, kBtnTakePicYOffSet),
                            "OK", kDarkPink, kTitleFont, kLightPink, kStringTitleScaler);
    camera_retakephoto_button_.setUp(window_w, window_h, kBtnRetakePicWidth, kBtnTakePicHeight,
                                     kBtnRoundSmall,
                                     ofPoint(kBtnRetakePicXOffset, kBtnRetakePicYOffset),
                                     "Retake Picture", kDarkPink, kTitleFont, kOffWhite,
                                     kStringContentScaler);
    camera_toimage_button_.setUp(window_w, window_h, kBtnRetakePicWidth, kBtnTakePicHeight,
                                 kBtnRoundSmall,
                                 ofPoint(kBtnUseThisPicXOffset, kBtnRetakePicYOffset),
                                 "UseThis Picture", kLightGreen, kTitleFont, kDarkGreen,
                                 kStringContentScaler);
    // buttons in IMGAE mode
    image_reset_button_.setUp(window_w, window_h, kBtnResetWidth, kBtnResetHeight, kBtnRoundSmall,
                              ofPoint(kBtnResetXOffset, YOffset_(kBtnResetYOffset)),
                              "Reset", kDarkPink, kContentFont, kOffWhite, kStringContentScaler);
    image_left_arro_button_.setUp(window_w, window_h,
                                  kBtnArrowWidth, kBtnArrowHeight, kBtnRoundSmall,
                                  ofPoint(kBtnLeftArroXOffset, YOffset_(kBtnArroYOffset)),
                                  "<", kDarkGreen, kArrowFont, kLightGreen, kStringTitleScaler);
    image_right_arro_button_.setUp(window_w, window_h,
                                   kBtnArrowWidth, kBtnArrowHeight, kBtnRoundSmall,
                                   ofPoint(kBtnRightArroXOffset, YOffset_(kBtnArroYOffset)),
                                   ">", kDarkGreen, kArrowFont, kLightGreen, kStringTitleScaler);
    image_saveimage_button_.setUp(window_w, window_h,
                                  kBtnBottomGroupWidth, kBtnBottomGroupHeight, kBtnRoundSmall,
                                  ofPoint(kBtnBottomGroupXOffset, YOffset_(kBtnSaveImgYOffset)),
                                  "SaveImage", kLightGreen, kContentFont, kDarkGreen,
                                  kStringContentScaler);
    image_tocamera_button_.setUp(window_w, window_h,
                                 kBtnBottomGroupWidth, kBtnBottomGroupHeight, kBtnRoundSmall,
                                 ofPoint(kBtnBottomGroupXOffset, YOffset_(kBtnUseCamYOffset)),
                                 "UseCamera", kDarkPink, kContentFont, kOffWhite,
                                 kStringContentScaler);
    image_loadanother_button_.setUp(window_w, window_h,
                                    kBtnBottomGroupWidth, kBtnBottomGroupHeight, kBtnRoundSmall,
                                    ofPoint(kBtnBottomGroupXOffset, YOffset_(kBtnLoadImgYOffset)),
                                    "LoadImage", kDarkPink, kContentFont, kOffWhite,
                                    kStringContentScaler);
    image_click_image_txt_.setUp(window_w, window_h, kTextTitleWidth, kTextTitleHeight, 0,
                       ofPoint(kTextTitleXOffset, kClickImageYOffset),
                       "ClickOnTheImage!", kTransparent, kTitleFont, kDarkPink,
                       kStringContentScaler);
    image_background_txt_.setUp(window_w, window_h, kTextTitleWidth, kTextTitleHeight, 0,
                      ofPoint(kTextTitleXOffset, kBackgroundYOffset),
                      "Background", kTransparent, kTitleFont, kDarkGreen, kStringContentScaler);
    image_bg_number_txt_.setUp(window_w, window_h, kBgNumberWidth, kBgNumberHeight, 0,
                     ofPoint(kBgNumberXOffset, kBgNumberYOffset),
                     kDefaultBgString, kTransparent, kTitleFont, kDarkGreen, kStringContentScaler);
    image_processing_overlap_.setUp(window_w, window_h, kImageSizeScalerW, kImageSizeScalerH, 0,
                                    ofPoint(kImageOffsetScalerX, kImageOffsetScalerY),
                                    "Processing...", kTransparent, kTitleFont, kGray,
                                    kStringProcessingScaler);
    
    // add all these buttons to an array so that they are easier to manage
    image_buttons_ = {&image_reset_button_, &image_left_arro_button_,
        &image_right_arro_button_, &image_saveimage_button_, &image_loadanother_button_,
        &image_tocamera_button_, &image_click_image_txt_, &image_background_txt_,
        &image_bg_number_txt_};
}
