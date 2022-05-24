# Projet TIFO oil painting webcam filter

### Sources utilisées pour développer le projet

https://github.com/umlaeute/v4l2loopback/blob/main/examples/yuv4mpeg_to_v4l2.c  
http://supercomputingblog.com/graphics/oil-painting-algorithm/  
https://www.codeproject.com/Articles/471994/OilPaintEffect

### Prérequis


### Comment build le projet ?

    $ mkdir build & cd build
    $ cmake .. -DOpenCV_DIR=path

Pour le filtre sur png :

    $ make pngfilter
    $ ./pngfilter --input pathIn --output pathOut

Pour le filtre webcam : 

    $ make webcamfilter
    $ sudo modprobe v4l2loopback
    $ ffmpeg -hide_banner -loglevel error -f v4l2 -r 25 -an  -i /dev/video0 -pix_fmt yuv420p \
    -f yuv4mpegpipe - | ./webcamfilter /dev/video2 (frame_delay)
    $ vlc v4l2:///dev/video2

Pour le filtre video :

    $ make videofilter
    $ ./videofilter videoIn videoOut framerate

Le framerate est littéralement le nombre d'images casées en une seconde, donc au mieux il faudrait mettre le meme que 
la vidéo originale sinon la vidéo va être accélérée/ralentie...