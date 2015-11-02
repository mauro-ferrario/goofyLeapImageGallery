#!/bin/sh

open goofyLeapImageGalleryDebug.app && kill -9 ` ps ax | grep goofyLeapImageGalleryDebug | cut -d' ' -f3`
cd /Users/Mauro/C++/of_v0.8.4_osx_release/apps/myApps/goofyLeapImageGallery/bin/
until [ `open -W goofyLeapImageGalleryDebug.app` ]; do
echo "!!- CRASHED -!! restarting..."
#sleep 1
done

