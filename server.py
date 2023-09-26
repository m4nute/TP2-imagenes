from fastapi import FastAPI, File, UploadFile
from fastapi.responses import JSONResponse
from fastapi.middleware.cors import CORSMiddleware
import subprocess
import os

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_methods=["*"],
    allow_headers=["*"],
)

upload_dir = "imagenes_subidas"
os.makedirs(upload_dir, exist_ok=True)
procces_dir = "imagenes_procesadas"
os.makedirs(procces_dir, exist_ok=True)

@app.post("/upload/")
async def upload_image(params: Upload):
    try:
        # Check if the file is an image (you can add more strict checks here)
        if params.file.filename.endswith((".jpg", ".jpeg", ".png", ".webp")):
            # Generate a unique filename
            file_name = os.path.join(upload_dir, params.file.filename)
            with open(file_name, "wb") as image_file:
                image_file.write(params.file.file.read())

        

        command = f"./main {params.filtro} {params.threads} {params.parametro} ./imagenes_subidas/{file_name} imagenes_procesadas"
        try:
            # Run the command and capture the output
            time = subprocess.check_output(command, shell=True, text=True)
            
            # Print the output
            print("C++ script output:")
            print(time)

        except subprocess.CalledProcessError as e:
            print(f"Error running C++ script: {e}")


            return JSONResponse(content={"message": "Image uploaded successfully"}, status_code=200)
        else:
            return JSONResponse(content={"message": "Invalid file format. Only images are allowed."}, status_code=400)
    except Exception as e:
        return JSONResponse(content={"message": f"An error occurred: {str(e)}"}, status_code=500)


