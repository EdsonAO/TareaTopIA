*Tareas a presentar para Tópicos de Inteligencia Artificial*
### Alumno: Edson Arratea Ope
## Backpropagation
Para entrenar la red neuronal se usan tres funciones, la función constructora de la clase *BackProp*, la función *Start* y la función *Train*.
En el archivo *main.cpp* se encuentran las siguientes funciones a explicar:
- **BackProp**
  Es la función constructora de la clase **BackProp A(p1,p2)**, esta función recive dos parámetros el primero(**p1**) es el *umbral*, el cual indica hasta cuanto tiene que disminuir el error del entrenamiento; el segundo(**p2**) parámetro es la *Tasa de Aprendizaje*. 

- **Start**
  La función **A.Start(p1,p2,p3,p4)**, recibe cuatro parámetros; **p1** es el tamaño de los inputs en el caso del **XOR** solo serían dos(*p,q*); **p2** es el número de neuronas de salida, en el caso del **XOR** solo es una salida(*0,1*); **p3** es un vector de capas ocultas, el tamaño del vector es la cantidad de capas ocultas y los números que se almacenan en el vector es la cantidad de neuronas por capa, por ejemplo {4,2} serían dos capas ocultas la primera con cuatro neuronas y la segunda con dos neuronas; **p4** es un vector del mismo tamaño del vector de capas el cual indica la función de activación por cada capa de acuerdo al número que se le indique, en el caso que el número no exista se usa la función *Sigmoidal*.
  - **1**: *Sigmoidal*
  - **2**: *ReLU*
  - **3**: *TanH*
  - **4**: *Raíz*

- **Train**
  La función **A.Train(p1,p2)** recibe dos parámetros; **p1** el cual es un vector de vectores con las entradas que se quiere entrenar, cabe resaltar que a cada entrada se le debe aumentar el *bias* que es un 1 al inicio; **p2** es un vector de vectores con las salidas deseadas en el mismo orden que sus entradas.  

Finalmente para realizar el test se usa la función **A.Test(p1)**, esta función sólo recibe **p1** que indica la entrada que se quiere testear, también debemos agregarle el bias al inicio. Al ejecutar **main.cpp** cuando el error sea mas pequeño que el *umbral* acaba el entrenamiento y se ejecutan los tests los cuales dan valores cercanos a 0 y 1 que corresponden al **XOR**.

## BWapi Starcraft

Para instalar el **BWapi** se siguió el tutorial de [GitHub BWapi](https://github.com/bwapi/bwapi) y se utilizó un sistema operativo *Windows 10* de 64 bits, los siguientes programas son necesarios para la instalación.

- **Visual Studio**
    En mi caso utilicé la versión *Ultimate 2013*
-  **StarCraft: Brood War**
   Obligatoriamente debe ser la versión 1.16.1
-  **BWAPI**
   Como se instaló Visual Studio 2013, se tiene que instalar *BWapi* en la versión 4.1.2 y se puede descargar de los [Releases](https://github.com/bwapi/bwapi/releases), al instalar tuve problemas porque no reconocía la carpeta del *Starcraft*, esto se soluciona instalando el *BWapi* a la misma altura del *Starcraft*. 
- **TEST**
Una vez instalado el *BWapi* debería haber creado una carpeta **bwapi-data** dentro del directorio de *Starcraft*.
    - Debemos ir a la carpeta donde instalamos el *BWapi* y compilar en modo Release con *Visual Studio* el archivo **ExampleProjects.sln**, el cual nos generará una carpeta *Release* dentro de *BWapi* en la cual se encuentra un **DLL** que se llama *ExampleAIModule.dll*.
    - *ExampleAIModule.dll* debemos copiarlo dentro de la carpeta del *Starcraft* dentro de *bwapi-data/AI*, la carpeta **AI** se debe crear manualmente.
    - Después dentro de la carpeta *BWAPI/chaoslauncher* lanzaremos como administrador *Chaoslauncher.exe*, debemos activar la casilla *BWAPI Injector x.x.x [RELEASE]* y clickear start.
    - Dentro del juego nos vamos a *Single Player* **->** *Expansion*, seleccionamos un usuario o creamos uno nuevo y le damos Ok.
    - Luego clickeamos *Play Custom*, seleccionamos un mapa e iniciamos el juego.
    - Ya en la partida se muestra un mensaje de *Hello world!* y los bots jugando solos.

  