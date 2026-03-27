SUMMARY = "Compile and install c app to run on boot"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

inherit systemd
SYSTEMD_SERVICE:${PN} = "bootapp.service"
SYSTEMD_AUTO_ENABLE:${PN} = "enable"

DEPENDS = "i2c-tools paho-mqtt-c"

SRC_URI =  "file://bootapp.c \
            file://bootapp.service \
            file://mqtt.h \
            file://mqtt.c \
"
S = "${UNPACKDIR}"

do_compile() {
    ${CC} ${CFLAGS} ${LDFLAGS} -I${STAGING_INCDIR} -o bootapp ${S}/bootapp.c ${S}/mqtt.c -lpaho-mqtt3c 
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 bootapp ${D}${bindir}

    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${UNPACKDIR}/bootapp.service ${D}${systemd_system_unitdir} 
    
    install -d ${D}${sysconfdir}/modules-load.d
    echo "i2c-dev" > ${D}${sysconfdir}/modules-load.d/i2c.conf
    echo "i2c-bcm2708" >> ${D}${sysconfdir}/modules-load.d/i2c.conf
}
