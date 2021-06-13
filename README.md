# FSP_ImageDeconvolution
FroSigPro Image Deconvolution


## Building Instructions

_These instructions have been tested in Ubuntu 18.04 & Ubuntu 20.04._

The following packages need to be installed in your machine in order to be able to build and run the `fsp_imgdcnv` application.



### Package installation
First update your local repository 

`$ sudo apt-get update`

Install the following packes:

#### C++ compiler and build tools
`$ sudo apt-get install build-essential`

####  Qt5
`$ sudo apt-get install qt5-default`

#### Armadillo
`$ sudo apt-get install libarmadillo-dev`

#### libpng
`$ sudo apt-get install libpng-dev`

#### libzip
`$ sudo apt-get install libzip-dev`

### Build application

In order to build the application you need to download the source code first.

Open your terminal and navigate to the folder you want to copy the project into.

You can use

`$ git clone https://github.com/h4koo/FSP_ImageDeconvolution.git` 

to download the source to this folder.

Navigate to the project folder

`$ cd FSP_ImageDeconvolution`

Run `qmake` to generate the makefile

`$ qmake`

After that run `make`   to build the application

`$ make`

Running the build instructions will create the `~/.fsp_imgdcnv` folder in the users home folder to store the calculated filters. It will also create the `build` directory where the executable and object files are stored during compilation.

### Run the application

Once the application has been build (hopefully without warnings or errors), you can navigate to the `build/bin` directory and run the application by using `./fsp_imgdcnv`.

```
$ cd build/bin
$ ./fsp_imgdcnv
```
