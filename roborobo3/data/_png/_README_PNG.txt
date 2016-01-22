Note, 2015-10-07: 

As of today, roborobo only supports BMP images (note that previous versions supported PNG). The reason is that there is a bug in SDL2_image, that prevent to load *exactly* the pixel values of the image, hence provoking problems with masks and images, due to automatic embedded calibration applied with SDL2_image's IMG_Load(...) function is called. 

cf. https://forums.libsdl.org/viewtopic.php?t=10013&sid=34e8ef7cf74074039495037c1a077025
cf. http://stackoverflow.com/questions/24987165/sdl2-sdl-image-strange-png-behavior-with-rgb-values
