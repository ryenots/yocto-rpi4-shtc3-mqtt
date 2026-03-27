FILESEXTRAPATHS:prepend := "${THISDIR}/mosquitto:"

SRC_URI:append = " file://mosquitto.conf"

do_install:append(){
    install -d ${D}${sysconfdir}/mosquitto
    install -m 0644 ${UNPACKDIR}/mosquitto.conf ${D}${sysconfdir}/mosquitto/mosquitto.conf
}