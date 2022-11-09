import sys
import os
import shutil

from PIL import Image

if len(sys.argv) <= 1:
    print("ERROR: no platform specified for asset building\n")
    sys.exit()

platform = sys.argv[1]

asset_out_dir = "binary/" + platform + "/assets/"
asset_dir = "assets/"

# delete the old assets if they already exist
if os.path.exists(asset_out_dir):
    shutil.rmtree(asset_out_dir)

# build textures
texture_out_dir = asset_out_dir + "textures/"
texture_in_dir = asset_dir + "textures/"

os.makedirs(texture_out_dir)

texture_files = os.listdir(path=texture_in_dir)
for file in texture_files:
    infile = os.path.join(texture_in_dir, file)
    if os.path.isfile(infile):
        if infile.split(".")[1] == "psd":
            outfile = str.replace(os.path.join(texture_out_dir, file), ".psd", ".png")
            print("converting texture {} to {}".format(infile, outfile))
            img = Image.open(infile)
            img.save(outfile)

# build shaders
shader_out_dir = asset_out_dir + "shaders/"
shader_in_dir = asset_dir + "shaders/"

os.makedirs(shader_out_dir)

shader_files = os.listdir(path=shader_in_dir)
for file in shader_files:
    infile = os.path.join(shader_in_dir, file)
    if os.path.isfile(infile):
        if infile.split(".")[1] == "shader":
            outfile = os.path.join(shader_out_dir, file)
            print("converting shader {} to {}".format(infile, outfile))
            with open(infile, "r") as inshader:
                src = inshader.read()
                # the type of shader output depends on the platform
                if platform == "win32":
                    with open(outfile, "w") as outshader:
                        outshader.write(src)
                if platform == "web":
                    src = str.replace(src, "layout (location = 0) ", "")
                    src = "precision mediump float;\n\n" + src
                    with open(outfile, "w") as outshader:
                        outshader.write(src)
