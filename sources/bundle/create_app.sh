#!/bin/sh
cd `dirname $0`

#クライアント

APP_NAME="SuperCartRacing11"
if [ $# -ge 1 ]; then
    APP_NAME=$1
fi
if [ -e ../${APP_NAME}.app ]; then
    rm -r ../${APP_NAME}.app
fi

mkdir "../${APP_NAME}.app"
mkdir "../${APP_NAME}.app/Contents"
mkdir "../${APP_NAME}.app/Contents/MacOS"
mkdir "../${APP_NAME}.app/Contents/Resources"
cp Info.plist "../${APP_NAME}.app/Contents/"
cp ../../resources/icon.icns "../${APP_NAME}.app/Contents/Resources/"
cp script.sh "../${APP_NAME}.app/Contents/MacOS/"


SUB_APP_DIR="../${APP_NAME}.app/Contents/MacOS/client.app"
mkdir ${SUB_APP_DIR}
mkdir "${SUB_APP_DIR}/Contents"
mkdir "${SUB_APP_DIR}/Contents/MacOS"
mkdir "${SUB_APP_DIR}/Contents/Resources"
cp -r ../../resources/* "${SUB_APP_DIR}/Contents/Resources/"
cp Info.plist "${SUB_APP_DIR}/Contents/"
cp ../Release/client.out "${SUB_APP_DIR}/Contents/MacOS/"
cp script2.sh "${SUB_APP_DIR}/Contents/MacOS/script.sh"


#サーバー

APP_NAME="SuperCartRacing11_Server"
if [ $# -ge 1 ]; then
    APP_NAME=$1"_Server"
fi
if [ -e ../${APP_NAME}.app ]; then
    rm -r ../${APP_NAME}.app
fi

mkdir "../${APP_NAME}.app"
mkdir "../${APP_NAME}.app/Contents"
mkdir "../${APP_NAME}.app/Contents/MacOS"
mkdir "../${APP_NAME}.app/Contents/Resources"
cp Info.plist_sv "../${APP_NAME}.app/Contents/Info.plist"
cp ../../resources/icon.icns "../${APP_NAME}.app/Contents/Resources/icon.icns"
cp script_sv.sh "../${APP_NAME}.app/Contents/MacOS/script.sh"


SUB_APP_DIR="../${APP_NAME}.app/Contents/MacOS/server.app"
mkdir ${SUB_APP_DIR}
mkdir "${SUB_APP_DIR}/Contents"
mkdir "${SUB_APP_DIR}/Contents/MacOS"
mkdir "${SUB_APP_DIR}/Contents/Resources"
cp -r ../../resources/* "${SUB_APP_DIR}/Contents/Resources/"
cp Info.plist_sv "${SUB_APP_DIR}/Contents/Info.plist"
cp ../Release/server.out "${SUB_APP_DIR}/Contents/MacOS/"
cp script2_sv.sh "${SUB_APP_DIR}/Contents/MacOS/script.sh"
