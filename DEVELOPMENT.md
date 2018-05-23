# Development log
## April 17 2018
- [x] Downloaded ofxCv but the examples don't compile. (solved by downloading the stable version)

## April 18 2018
- [x] Buttons are not drawn (add it to gui and draw the gui)
- [x] Target color is fixed, which makes it only recognizes part of objects (Pick points along the edge to extend)
- [x] Buttons don't look like buttons. Change the font, size. (Change from gui button to rectangle)
- [x] Fix all the magic numbers from the example.
- [x] The two buttons are not on the top left corner.

## April 24 2018
- [x] String on menu buttons are not properly aligned (Using addon ofxParagraph)
- [x] Random number generator doesn't seem to work, test it. (Used)
- [x] Convex hull is covering too much region, but original polyline is too spiky. Find a intermediate. (Used smooth)
- [x] Extract functions from process image.

## April 30 2018
- [x] Buttons are not resizing, and positions are not relative (Wrote a RoundedButton class)
- [x] Texts on the button are drawn off position (It turns out the y position is at the bottom of the first line of text)

## May 1 2018
- [x] Image processing takes too long (Resize the image to be smaller)
- [x] When extending along the edge, the code seems to run into a infinite loop (Add the point on the edge to new_highlight solves it)
- [x] Can't draw transparent buttons in CAMERA mode (Didn't figure out why, so only drew solid color buttons)

## May 2
- [x] User can't tell whether the image is being processed or not (Added "process..." when processing)
