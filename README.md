# Projet TIFO oil painting webcam filter

## Sources utilisées pour développer le projet

https://github.com/umlaeute/v4l2loopback/blob/main/examples/yuv4mpeg_to_v4l2.c  
http://supercomputingblog.com/graphics/oil-painting-algorithm/  
https://www.codeproject.com/Articles/471994/OilPaintEffect

## Prérequis

Pour la version CPU :
* ffmpeg
* v4l2 pour tester la webcam
* Une installation openCV

Pour la version GPU :
* ffmpeg
* v4l2 pour tester la webcam
* nvcc (évidemment)


## Comment build le projet ?

### Version GPU

ask Pejman

### Version CPU

Est contenue dans src/

    $ mkdir build & cd build
    $ cmake .. -DOpenCV_DIR=path

Pour le filtre sur png :

    $ make pngfilter
    $ ./pngfilter --input pathIn --output pathOut

Pour le filtre video :

    $ make videofilter
    $ ./videofilter videoIn videoOut framerate

Example rendu : https://youtu.be/ie_uuXbl8fI

Le framerate est littéralement le nombre d'images casées en une seconde, donc au mieux il faudrait mettre le meme que 
la vidéo originale sinon la vidéo va être accélérée/ralentie...  
Ca tourne sur cpu donc c'est très long...