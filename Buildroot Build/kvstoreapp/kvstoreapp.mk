################################################################################
#
# kvstoreapp
#
################################################################################

KVSTOREAPP_VERSION = 1.0
KVSTOREAPP_SITE = ./package/kvstoreapp
KVSTOREAPP_SITE_METHOD = local

ifeq ($(BR2_PACKAGE_LEVELDB),y)
KVSTOREAPP_DEPENDENCIES += leveldb
endif

define KVSTOREAPP_BUILD_CMDS
    $(MAKE) CC="$(TARGET_CC)" LD="$(TARGET_LD)" -C $(@D) all
endef

define KVSTOREAPP_INSTALL_TARGET_CMDS
    $(INSTALL) -D -m 0755 $(@D)/kvstoreapp $(TARGET_DIR)/usr/bin
endef

$(eval $(generic-package))
