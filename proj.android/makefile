all:
	./build_native.sh
	ant -Dsdk.dir=C:\\Users\\darkcl\\android-sdks debug
run:
	adb -d uninstall scm.yiuhyeung2.brick_cross
	adb -d install bin/BrickCross-debug.apk
	adb shell am start -n scm.yiuhyeung2.brick_cross
	adb -d logcat | grep cocos2d
clean:
	rm -rf libs/
	rm -rf obj/
	rm -rf bin/*
	rm -rf gen/
	rm -rf assets/*
