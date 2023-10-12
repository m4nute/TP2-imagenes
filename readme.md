# TP4

# Front End
-  El front end se encuentra dentro del directorio front-imagenes
-  Para acceder a el fácilmente, ejecutar 'cd front-imagenes'
## Instalar bun
-  npm install -g bun
-  (Bun es un entorno en tiempo de ejecución de JavaScript, como Node o Deno, todo en uno. Bun tiene un paquete nativo, un transpilador, un ejecutor de tareas y un cliente npm integrado.)

## Descargar dependencias
- bun install
- (Instala dependencias como tailwind y una libreria de componentes.)

  

## Correr el entorno de desarrollo de svelte
- bun dev (se ejecuta en localhost:5173)

# Back End
-  Si hiciste 'cd front-imagenes', ejecutar 'cd ..' para regresar al rootdir.
## Descargar dependencias
- (opcional, hacer venv con "python -m venv venv" y "source venv/bin/activate")
- pip install -r requirements.txt
- (Instalar paquetes relacionados con FastAPI, python-multipart para recibir archivos mediante http y PIL (Pillow) para conversion de tipos de archivos de imagen.)

## Correr server de FastAPI
- $ uvicorn server:app --reload
