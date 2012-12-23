BrickCross
==========
place photon c++ lib in root of cocos2dx folder

For win32:vcproject may need to set heard path to your absoulte path to the library and include folder, the paths are inclued in photon development document, also you may use this blog as reference

http://codebygeorgeguy.wordpress.com/

For android: in proj.android/bulid_native.sh you need to set the NDK root to your absoulte path to your ndk root. a make file is include in the project, if you want to use the make file, you need to set sdk dir to your own dir



REFERENCE:

Photon + cocos2dx setup : http://codebygeorgeguy.wordpress.com/

Photon Library 
(use some class in :Demo/demo_realtime & Demo/demo_loadingBalance) : http://exitgames.com/

cocos2dx (using version 2.0.4) : http://www.cocos2d-x.org/ 
