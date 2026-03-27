require recipes-core/images/core-image-minimal.bb

IMAGE_INSTALL:append = " openssh i2c-tools mosquitto paho-mqtt-c bootapp"

EXTRA_IMAGE_FEATURES += " allow-root-login allow-empty-password"

inherit extrausers

EXTRA_USERS_PARAMS = "usermod -p '\$6\$SfBmQbuRzyb8Tkdv\$eDwD1ifCkvhl07obRVbWf0Po4RyH56jPXzS6MHq0YAbboc2BaSNeMe3GOALAGea8cDUvgqhitMUg8ELDkQbfS.' root; \
    useradd -m -s /bin/sh -p '\$6\$SfBmQbuRzyb8Tkdv\$eDwD1ifCkvhl07obRVbWf0Po4RyH56jPXzS6MHq0YAbboc2BaSNeMe3GOALAGea8cDUvgqhitMUg8ELDkQbfS.' ryenots; \
"
