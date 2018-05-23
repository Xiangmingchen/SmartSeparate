#include "ofSmartSeparate.h"

int main() {
    ofGLFWWindowSettings setting;
    setting.windowMode = OF_WINDOW;
    auto window = ofCreateWindow(setting);
    ofSetVerticalSync(true);
    ofSetFrameRate(25);
    
    ofPoint screen_size = window->getScreenSize();
	ofRunApp(new ofSmartSeparate(screen_size));
}
