TEMPLATE 	= subdirs
SUBDIRS 	= global \
    manjaro-settings-manager \
    manjaro-settings-manager-daemon
    

OPTIONS 	+= ordered




binfiles.path = /usr/bin/
binfiles.files = data/bin/*

varfiles.path = /var/lib/manjaro-system-settings/
varfiles.files = data/var/*

xdgautostart.path = /etc/xdg/autostart/
xdgautostart.files = data/autostart/*

appfiles.path = /usr/share/applications/
appfiles.files = data/applications/*

INSTALLS += binfiles varfiles xdgautostart appfiles
