#!/bin/sh

./getPic 1
cp 2.pnm ../image_rec/hough
cd ../image_rec/hough; ./circleFinder; eog findCenter.jpg

