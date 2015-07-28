# Manjaro Settings Manager

The Manjaro Settings Manager offers you a series of settings, which are
either enabled by Manjaro (i.e. installation of multiple kernels) or are 
missing from at least some of popular Desktop Environments and Window Managers.

Currently has modules written for Manjaro Hardware Detection (MHWD), Language,
Kernel, Keyboard, Time and Date and User Accounts.

It also includes a daemon to notify user of new language packages or kernels.

Manjaro Settings Manager is under active development.


### BUILD INSTRUCTIONS

~> makepkg -s

   Install missing dependencies using pacman. When build-time or run-time
   dependencies are not found, pacman will try to resolve them. If successful,
   the missing packages will be downloaded and installed. (If the package was
   already built before you may use -sf to overite the previous build)


~> pacman -U <package-name>
  
   Upgrade or add package(s) to the system and install the required 
   dependencies from sync repositories. Enter the path to the build file (.pkg.
   tar.xz) here. This is a “remove-then-add” process. 

Now the build is complete and you can run it using `msm` command in terminal.


### RESOURCES

* Website        https://manjaro.github.io/
* Wiki           https://wiki.manjaro.org/
* Mailing List   https://lists.manjaro.org/mailman/listinfo
* Download       http://manjaro.github.io/download/
* IRC            #manjaro (www.freenode.com)
