# Projet TIFO oil painting webcam filter

## Sources utilisées pour développer le projet

## Prérequis
* nvcc (évidemment)
* opencv
* libpng

## Comment build le projet ?

### Version GPU
```bash
mkdir build && cd build
cmake ..
make render -j8
```

## Comment lancer le projet ?
```bash
# Appliquer le filtre sur une image
./render -i path/to/img.png -m image

# Appliquer le filtre sur la webcam
./render -m webcam
```

