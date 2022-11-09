import os

from PIL import Image

outdir = "assets/textures/"
indir = "asset_dev/"

if not os.path.isdir(outdir):
    os.makedirs(outdir)

files = os.listdir(path=indir)
for file in files:
    infile = os.path.join(indir, file)
    if os.path.isfile(infile):
        if infile.split(".")[1] == "psd":
            outfile = str.replace(os.path.join(outdir, file), ".psd", ".png")
            print("converting texture {} to {}".format(infile, outfile))
            img = Image.open(infile)
            img.save(outfile)
