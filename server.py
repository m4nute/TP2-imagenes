from pydantic import BaseModel
from fastapi import FastAPI, File, UploadFile, Form
from fastapi.responses import JSONResponse, FileResponse
import subprocess
from fastapi.middleware.cors import CORSMiddleware
from typing_extensions import Annotated
import os
from PIL import Image


app = FastAPI()
origins = ["http://localhost:5173"]

app.add_middleware(
    CORSMiddleware,
    allow_origins=origins,
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
    expose_headers=["x-time"]
)
upload_dir = "imagenes_subidas"
os.makedirs(upload_dir, exist_ok=True)
procces_dir = "imagenes_procesadas"
os.makedirs(procces_dir, exist_ok=True)

@app.post("/upload/")
async def upload_image(file: UploadFile = File(...), filtro: Annotated[str, Form()] = 'plain', threads: Annotated[str, Form()] = '1', parametro: Annotated[str, Form()] = '0'):
    remove_from_folder(upload_dir)
    remove_from_folder(procces_dir)
    time = ""
    if file.filename.endswith((".jpg", ".jpeg", ".png", ".webp")):
            # Generate a unique filename
        
        file_name = os.path.join(upload_dir, file.filename)
        with open(file_name, "wb") as image_file:
                image_file.write(file.file.read())
    
        with Image.open(file_name) as im:
            im.save(file_name.split(".")[0]+".ppm")
        
        command = ["./main", filtro, threads, parametro, file_name.split('.')[0] + '.ppm', "./imagenes_procesadas/" + file.filename.split(".")[0] + ".ppm"]
        try:
             
            time = subprocess.check_output(command, text=True)
            
            print("C++ script output:")
            print(time)
            
            with Image.open("./imagenes_procesadas/" + file.filename.split(".")[0] + ".ppm") as im2:
                im2.save("./imagenes_procesadas/" + file.filename)

        except subprocess.CalledProcessError as e:
            print(f"Error running C++ script: {e}")


        response = FileResponse("./imagenes_procesadas/" + file.filename, media_type='application/octet-stream', filename="result.jpg")
        response.headers["x-time"] = time.split("\nE")[0]

        return response
        # return JSONResponse(content={"message": "Image uploaded successfully", "file_done": }, status_code=200)
    else:
        pass    


@app.post("/upload/merge")
async def upload_image(file1: UploadFile = File(...),file2: UploadFile = File(...) , filtro: Annotated[str, Form()] = 'plain', threads: Annotated[str, Form()] = '1', parametro: Annotated[str, Form()] = '0'):
    remove_from_folder(upload_dir)
    remove_from_folder(procces_dir)

    if file1.filename.endswith((".jpg", ".jpeg", ".png", ".webp")) and file2.filename.endswith((".jpg", ".jpeg", ".png", ".webp")):
            # Generate a unique filename
        file_name = os.path.join(upload_dir, file1.filename)
        with open(file_name, "wb") as image_file:
                image_file.write(file1.file.read())
        
        file_name_2 = os.path.join(upload_dir, file2.filename)
        with open(file_name_2, "wb") as image_file:
                image_file.write(file2.file.read())
    
        with Image.open(file_name) as im:
            im.save(file_name.split(".")[0]+".ppm")

        with Image.open(file_name_2) as im:
            im.save(file_name_2.split(".")[0]+".ppm")
        
        command = ["./main", filtro, threads, parametro, file_name.split('.')[0] + '.ppm', "./imagenes_procesadas/result.ppm", file_name_2.split('.')[0] + '.ppm']


        try:
             
            time = subprocess.check_output(command, text=True)
            
            print("C++ script output:")
            print(time)
            
            with Image.open("./imagenes_procesadas/result.ppm") as im2:
                im2.save("./imagenes_procesadas/result." + file1.filename.split(".")[1])

        except subprocess.CalledProcessError as e:
            print(f"Error running C++ script: {e}")

        headers = {
            "x-time": time.split("\nE")[0],
        }

        return FileResponse("./imagenes_procesadas/result." + file1.filename.split(".")[1], media_type='application/octet-stream', filename=("result" + file1.filename.split(".")[1]), headers=headers)
    else:
        pass    

def remove_from_folder(directory_path):
    file_list = os.listdir(directory_path)

    # Loop through the list of files and remove each one
    for filename in file_list:
        file_path = os.path.join(directory_path, filename)
        
        # Check if the path is a file (not a subdirectory)
        if os.path.isfile(file_path):
            # Remove the file
            os.remove(file_path)