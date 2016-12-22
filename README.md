# create_dataset

This program is intended to create image training datasets for machine and deep learnings applications.

Description
-----------

Given the input and output directory this code generates X random transformations for each image of the input directory. 
The patch size used is fixed to the same as the images at input directory.

This code requires OpenCV!

Example
-------

In this case we want to train a convolutional neural network (CNN) for keypoint matching task.
Given the patches at input dir this code generate the following "random" transformations for them. 
![Sample of transformations generated](https://raw.github.com/mondejar/create_dataset/master/sample.png "Sample of transformations generated")


Compile
-------
```
mkdir build
cd build
cmake ..
make
```

How to use
----------
```
./create_dataset path_input path_out X
```

