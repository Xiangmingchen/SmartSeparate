//
//  roundedButton.h
//  final-project-Xiangmingchen
//
//  Created by Jenny Chen on 30/04/2018.
//

#ifndef rounded_button_h
#define rounded_button_h

#include "ofMain.h"
#include "ofxParagraph.h"

// A Rounded Button class in the shape of a rounded rectangle. It's width, height,
// position, and text size are all relative to the window dimension
class RoundedButton {
public:
    // Constructor that takes in all necessary parameters
    void setUp(double window_w, double window_h,
               double percent_width, double percent_height,
               double percent_round,                   // default not rounded
               ofPoint percent_left_top_corner,
               std::string text,
               ofColor background_color,
               std::string font_name,
               ofColor text_color,
               double string_scaler);  // default color is gray
    
    void draw();                     // draw this button
    bool contains(float x, float y); // check whether a point is on this button
    
    // setters
    void SetText(std::string new_text);
    void SetPercentWidth(double new_percent_width);
    void SetPercentHeight(double new_percent_height);
    void SetPosition(ofPoint percent_left_top_corner);
    void ResizeWindow(double window_w, double window_h);
    
private:
    ofRectangle button_;
    
    void update_();  // update rectangle position and size based on current change
    
    // window width and height
    double window_w_;
    double window_h_;
    
    // the top left coordinates of text
    double text_x_;
    double text_y_;
    
    // percent width, height, and rounded relative to window
    double percent_width_;
    double percent_height_;
    double percent_round_;
    
    ofxParagraph font_;
    ofPoint left_top_;
    ofColor bg_color_;
    ofColor text_color_;
    std::string font_name_;
    
    // constants
    const int kBaseSize = 10;
    double kPercentSpan = 2.0 / 3.0; // how much width text can span on the button
    double kLeftOffSet = 1.0 / 6.0;  // from where to the left to draw the button's text
    double kUpOffSet = 7;
    double kStringScaler;
};

#endif /* rounded_button_h */
