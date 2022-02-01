################################################################################
#
# kvstoreapp
#
################################################################################

KVSTORECLIENT_VERSION = 1.0
KVSTORECLIENT_SITE = ./package/kvstoreclient
KVSTORECLIENT_SITE_METHOD = local


define KVSTORECLIENT_BUILD_CMDS
    $(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D) all
endef

define KVSTORECLIENT_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/kvstoreclient $(TARGET_DIR)/usr/bin
endef

$(eval $(generic-package))
