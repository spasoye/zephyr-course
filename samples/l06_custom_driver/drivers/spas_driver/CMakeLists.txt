if (CONFIG_SPAS_DRIVER)
target_sources(app PRIVATE spas_driver.c)
target_include_directories(app PRIVATE .)
endif ()