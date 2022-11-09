import sys
import os
import shutil

from rectpack import newPacker
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

# build platform shaders
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

# crop temp textures
texture_out_dir = asset_out_dir + "textures/"
texture_in_dir  = asset_dir + "textures/"
texture_tmp_dir = texture_in_dir + "temp/"
texture_dat_dir = "source/codegen/"

if os.path.exists(texture_tmp_dir):
    shutil.rmtree(texture_tmp_dir)
if os.path.exists(texture_dat_dir):
    shutil.rmtree(texture_dat_dir)

os.makedirs(texture_out_dir)
os.makedirs(texture_tmp_dir)
os.makedirs(texture_dat_dir)

atlas_lists = {}

atlas_padding = 8

texture_files = os.listdir(path=texture_in_dir)
for file in texture_files:
    infile = os.path.join(texture_in_dir, file)
    if os.path.isfile(infile):
        if infile.split(".")[1] == "psd":
            # crop each input image and save it as a temporary png for atlas packing
            tmpfile = str.replace(os.path.join(texture_tmp_dir, file), ".psd", ".png")
            print("cropping texture {} to {}".format(infile, tmpfile))
            img = Image.open(infile)
            box = img.getbbox()
            img = img.crop(box)
            img.save(tmpfile)
            atlas_name = os.path.basename(tmpfile).split('_')[0].split('.')[0]
            if atlas_name not in atlas_lists:
                atlas_lists[atlas_name] = []
            atlas_lists[atlas_name].append((tmpfile,box))

# build texture atlases
for name,files in atlas_lists.items():
    if len(files) == 1:
        # save out the original
        infile = str.replace(os.path.join(texture_in_dir, os.path.basename(files[0][0])), ".png", ".psd")
        outfile = str.replace(os.path.join(texture_out_dir, os.path.basename(files[0][0])), ".psd", ".png")
        print("converting texture {} texture to {}".format(infile, outfile))
        img = Image.open(infile)
        img.save(outfile)
    else:
        outfile = os.path.join(texture_out_dir, name + ".png")
        print("building texture atlas {}".format(outfile))
        # load all of the textures
        textures = []
        rects = []
        for file in files:
            img = Image.open(file[0])
            w = img.size[0] + atlas_padding
            h = img.size[1] + atlas_padding
            textures.append(img)
            rects.append((w,h))
        # pack the texture rects
        for atlas_pow in range(1,12):
            packer = newPacker(rotation=False)
            for rect in rects:
                packer.add_rect(*rect)
            atlas_size = pow(2, atlas_pow)
            packer.add_bin(atlas_size,atlas_size)
            packer.pack()
            if len(packer) and len(packer[0]) == len(rects): # increas size until successful
                break
        # build the final atlas image
        atlas = Image.new("RGBA", (atlas_size,atlas_size))
        defines = []
        rectdat = []
        for i in range(len(textures)):
            offx = files[i][1][0]
            offy = files[i][1][1]
            x = int(packer[0][i].x + (atlas_padding / 2))
            y = int(packer[0][i].y + (atlas_padding / 2))
            w = packer[0][i].width
            h = packer[0][i].height
            atlas.paste(textures[i], (x,y))
            defines.append(os.path.basename(files[i][0]).split('.')[0].upper())
            rectdat.append("{{ {:>4},{:>4}, {{ {:>4},{:>4},{:>4},{:>4} }} }}".format(offx,offy, x,y,w,h))
        atlas.save(outfile)
        # write out the atlas rect data
        cdata  = "#ifndef ATLAS_{}_H__\n".format(name.upper())
        cdata += "#define ATLAS_{}_H__\n".format(name.upper())
        cdata += "\n"
        cdata += "// DO NOT EDIT: This file was auto generated as part of the asset build process.\n"
        cdata += "\n"
        for i in range(len(defines)):
            cdata += "#define ATLAS_{} {}\n".format(defines[i], i)
        cdata += "\n"
        cdata += "static const TextureAtlasClip ATLAS_{}[] =\n".format(name.upper())
        cdata += "{\n"
        for data in rectdat:
            cdata += "{},\n".format(data)
        cdata +="};\n"
        cdata +="\n"
        cdata +="#endif // ATLAS_{}_H__\n".format(name.upper())
        with open(os.path.join(texture_dat_dir, "atlas_" + name + ".h"), "w") as datafile:
            datafile.write(cdata)
