//
//  smartseparatetest.cpp
//  final-project-Xiangmingchen
//

#include "../src/ofSmartSeparate.h"
#include "../src/roundedButton.h"

#include "catch.hpp"

TEST_CASE("Rounded Button contains works properly") {
    RoundedButton test_button;
    test_button.setUp(100, 100, 1.0 / 2.0, 1.0 / 2.0, 0, ofPoint(1.0 / 4.0, 1.0 / 4.0), "",
                      {}, "Neon.ttf", {}, 0.01);

    REQUIRE(test_button.contains(50, 50));
    REQUIRE(test_button.contains(26, 26));
    REQUIRE(test_button.contains(26, 74));
    REQUIRE(test_button.contains(74, 26));
    REQUIRE(test_button.contains(74, 74));

    test_button.ResizeWindow(60, 60);
    REQUIRE(test_button.contains(13, 13));
}

TEST_CASE("App: CameraButtonSelected") {
    ofSmartSeparate test_app;

    test_app.CameraButtonSelected_();
    REQUIRE(test_app.current_state_ == ofSmartSeparate::State::CAMERA);
    REQUIRE(!test_app.picture_taken_);
}

