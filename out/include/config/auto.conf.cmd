deps_config := \
	main/Kconfig \
	lib/Kconfig \
	Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
