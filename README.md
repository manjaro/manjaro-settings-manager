# Manjaro Settings Manager

The Manjaro Settings Manager offers you a series of settings, which are
either enabled by Manjaro (i.e. installation of multiple kernels) or are 
missing from at least some of popular Desktop Environments and Window Managers.

Currently has modules written for Manjaro Hardware Detection (MHWD), Language,
Kernel, Keyboard, Time and Date and User Accounts.

It also includes a daemon to notify user of new language packages or kernels.

Manjaro Settings Manager is under active development.


### BUILD INSTRUCTIONS

    mkdir build  
    cd build  
    cmake ../ \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX=/usr \
        -DLIB_INSTALL_DIR=lib \
        -DKDE_INSTALL_USE_QT_SYS_PATHS=ON \
        -DSYSCONF_INSTALL_DIR=/etc
    make
    make install  
  
You can also use the provided PKGBUILD to compile and install it.
   
    makepkg -si


### DEPENDENCIES

* Qt5 >= 5.3.0
* KF5CoreAddons
* KF5Auth
* KF5ConfigWidgets
* KF5ItemModels
* KF5Notifications
* KF5KCMUtils
* KF5IconThemes


### EXECUTION

Now the build is complete and you can run it using `msm` command in terminal.

It will also show up the new kcm modules in kde's systemsettings or issuing the command:
`kcmshell5 msm_{kernel,keyboard,language_packages,locale,mhwd,notifications,timedate,users}`


### RESOURCES

* Website        https://manjaro.github.io/
* Wiki           https://wiki.manjaro.org/
* Mailing List   https://lists.manjaro.org/mailman/listinfo
* Download       http://manjaro.github.io/download/
* IRC            #manjaro (www.freenode.com)
