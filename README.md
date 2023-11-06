# Worms
Trabajo Práctico Worms - Taller de Programación I - Catedra Veiga

## Prerequisitos
Tener instalada la librería SDL2

## Compilacion y ejecución
Para la compilación se creará una carpeta build y se utilizará el cmake. Para ello en el directorio donde tenemos el cmake ejecutar:
```console
mkdir build
cd build
cmake ..
make
```
Esto creará 2 archivos binarios que podrán ser ejecutados abriendo 2 terminales y corriendo lo siguiente:

En una terminal:
```console
./server 9000
```

En la otra:
```console
./client localhost 9000
```
Cabe aclarar que se puede reemplazar `localhost` por `127.0.0.1` y `9000` por otro número siempre y cuando coincidan.

## Ejecución con tiburoncin

Con tiburoncin podemos ver los bytes enviados y recibidos, es una gran herramienta para debuggear y controlar que nuestro programa esté funcionando bien, para correrlo escribimos:

En una terminal:
```console
./server 9000
```

En otra:
```console
tiburoncin -o -A 127.0.0.1:9095 -B 127.0.0.1:9000
```

Y en una tercera:
```console
./client 127.0.0.1 9095
```

De esta forma tenemos a tiburoncin como intermediario entre cliente y servidor, mostrándonos los bytes enviados y recibidos.