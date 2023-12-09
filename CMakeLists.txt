# PSn00bSDK example CMake script
# (C) 2021 spicyjpeg - MPL licensed

cmake_minimum_required(VERSION 3.21)

project(
	PSn00bSDK-invaderz
	LANGUAGES    C CXX ASM
	VERSION      1.0.0
	DESCRIPTION  "Poopy stinky"
	HOMEPAGE_URL "http://lameguy64.net/?page=psn00bsdk"
)

psn00bsdk_add_executable(invaderz GPREL src/main.c src/core.c src/graphics.c src/font.c src/controller.c src/audio.c)

psn00bsdk_add_cd_image(
	iso      # Target name
	invaderz # Output file name (= invaderz.bin + invaderz.cue)
	iso.xml  # Path to config file
	DEPENDS invaderz system.cnf
)
