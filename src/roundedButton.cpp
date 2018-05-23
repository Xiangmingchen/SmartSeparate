//
//  roundedButton.cpp
//  final-project-Xiangmingchen
//
//  Created by Jenny Chen on 30/04/2018.
//

#include "roundedButton.h"

void RoundedButton::setUp(double window_w, double window_h,
                          double percent_width, double percent_height,
                          double percent_round,
                          ofPoint percent_left_top_corner,
                          std::string text,
                          ofColor background_color,
                          std::string font_name,
                          ofColor text_color,
                          double string_scaler) {
    // setting variables to the new value
    window_w_ = window_w;
    window_h_ = window_h;
    percent_width_ = percent_width;
    percent_height_ = percent_height;
    percent_round_ = percent_round;
    left_top_ = percent_left_top_corner;
    bg_color_ = background_color;
    font_name_ = font_name;
    text_color_ = text_color;
    font_.setText(text);
    font_.setAlignment(ofxParagraph::Alignment::ALIGN_CENTER);
    font_.setColor(text_color_);
    kStringScaler = string_scaler;
    
    // update based on the new values
    update_();
}

void RoundedButton::draw() {
    // draw the rectangle
    ofSetColor(bg_color_);
    ofDrawRectRounded(button_, window_w_ * percent_round_);
    // draw the text
    font_.draw(text_x_, text_y_);
}

bool RoundedButton::contains(float x, float y) {
    return button_.inside(x, y);
}

void RoundedButton::ResizeWindow(double window_w, double window_h) {
    window_w_ = window_w;
    window_h_ = window_h;
    update_();
}

void RoundedButton::update_() {
    button_.setPosition(window_w_ * left_top_.x, window_h_ * left_top_.y);
    button_.setSize(window_w_ * percent_width_, window_h_ * percent_height_);
    double font_size = kBaseSize + window_w_ * kStringScaler;
    font_.setWidth(button_.getWidth() * kPercentSpan);
    font_.setFont(font_name_, font_size);
    text_x_ = button_.getX() + button_.getWidth() * kLeftOffSet;
    text_y_ = button_.getY() + (button_.getHeight() - font_.getHeight()) / 2.0
              + font_size + kUpOffSet;
}

// setters
void RoundedButton::SetText(std::string new_text) {
    font_.setText(new_text);
}

void RoundedButton::SetPercentWidth(double new_percent_width) {
    percent_width_ = new_percent_width;
    update_();
}

void RoundedButton::SetPercentHeight(double new_percent_height) {
    percent_height_ = new_percent_height;
    update_();
}

void RoundedButton::SetPosition(ofPoint percent_left_top_corner) {
    left_top_ = percent_left_top_corner;
    update_();
}
