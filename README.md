# Projet TIFO oil painting webcam filter

https://github.com/umlaeute/v4l2loopback/blob/main/examples/yuv4mpeg_to_v4l2.c  
http://supercomputingblog.com/graphics/oil-painting-algorithm/  
https://www.codeproject.com/Articles/471994/OilPaintEffect

    $ sudo modprobe v4l2loopback
    $ ffmpeg -hide_banner -loglevel error -f v4l2 -r 25 -an  -i /dev/video0 -pix_fmt yuv420p \
    -f yuv4mpegpipe - | ./videofilter /dev/video2
    $ vlc v4l2:///dev/video2